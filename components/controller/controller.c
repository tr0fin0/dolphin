#include "config.h"
#include "controller.h"
#include "esc.h"
#include "logging.h"
#include <math.h>
#include "odometry.h"
#include "pid.h"
#include "pwm.h"

/**
 * @brief PID controller for the angular position.
 */
static pid_t pid_angular;

/**
 * @brief PID controller for the linear position.
 */
static pid_t pid_linear;

void controller_init(void) {
    pid_angular = (pid_t) {
        .kp             = CONTROLLER_PID_ANGULAR_KP,
        .ki             = CONTROLLER_PID_ANGULAR_KI,
        .kd             = CONTROLLER_PID_ANGULAR_KD,
        .error_i_max    = CONTROLLER_PID_ANGULAR_I_MAX,
    };

    pid_linear = (pid_t) {
        .kp             = CONTROLLER_PID_LINEAR_KP,
        .ki             = CONTROLLER_PID_LINEAR_KI,
        .kd             = CONTROLLER_PID_LINEAR_KD,
        .error_i_max    = CONTROLLER_PID_LINEAR_I_MAX,
    };
}

bool controller_navigate(odometry_waypoint_t current, odometry_waypoint_t target) {
    odometry_dis_m_t x_err_m         = target.x_m - current.x_m;
    odometry_dis_m_t y_err_m         = target.y_m - current.y_m;
    odometry_dis_m_t d_err_m         = sqrtf(
        (x_err_m * x_err_m) + (y_err_m * y_err_m)
    );
    odometry_ang_rad_t theta_err_rad = odometry_normalize_angle(
        atan2f(y_err_m, x_err_m) - current.theta_rad
    );

    if (
        (target.d_err_m       > d_err_m) &&
        (target.theta_err_rad > fabsf(theta_err_rad))
    ) {
        LOG_I("arrived at the target waypoint");
        esc_set_pwm_mix_neutral();

        return false;
    }

    odometry_vel_mps_t v_mps     = 0.0f;
    odometry_vel_rps_t omega_rps = 0.0f;

    if (d_err_m > target.d_err_m) {
        pid_angular.setpoint = +0.0f;
        pid_angular.current  = -theta_err_rad;
        pid_compute(&pid_angular);

        pid_linear.setpoint = +0.0f;
        pid_linear.current  = -d_err_m;
        pid_compute(&pid_linear);

        omega_rps = pid_angular.output;
        v_mps     = pid_linear.output;
    } else {
        theta_err_rad = odometry_normalize_angle(
            target.theta_rad - current.theta_rad
        );

        if (fabsf(theta_err_rad) < target.theta_err_rad) {
            LOG_I("arrived at the target waypoint");
            esc_set_pwm_mix_neutral();

            return false;
        }

        pid_angular.setpoint = +0.0f;
        pid_angular.current  = -theta_err_rad;
        pid_compute(&pid_angular);

        omega_rps = pid_angular.output;
        v_mps     = 0.0f;
    }

    if (v_mps     > +target.v_max_mps)     v_mps     = +target.v_max_mps;
    if (v_mps     < -target.v_max_mps)     v_mps     = -target.v_max_mps;
    if (omega_rps > +target.omega_max_rps) omega_rps = +target.omega_max_rps;
    if (omega_rps < -target.omega_max_rps) omega_rps = -target.omega_max_rps;

    odometry_vel_mps_t v_l_mps = v_mps - (omega_rps * CONFIG_WHEELBASE_M / 2.0f);
    odometry_vel_mps_t v_r_mps = v_mps + (omega_rps * CONFIG_WHEELBASE_M / 2.0f);

    pwm_t pwm_l = PWM_NEUTRAL_US + (v_l_mps / CONTROLLER_V_MAX_MPS) * 500.0f;
    pwm_t pwm_r = PWM_NEUTRAL_US + (v_r_mps / CONTROLLER_V_MAX_MPS) * 500.0f;
    
    esc_set_pwm(ESC_L, pwm_l);
    esc_set_pwm(ESC_R, pwm_r);

    return true;
}
