#include "config.h"
#include "encoder.h"
#include "esp_timer.h"
#include "logging.h"
#include <math.h>
#include "odometry.h"

/**
 * @brief Odometry of the system from encoder measures integration.
 */
static odometry_t odometry;

void odometry_get(odometry_t *current_odometry) {
    *current_odometry = odometry;
}

void odometry_init() {
    encoder_init();

    odometry = (odometry_t) {
        .x_m          = (odometry_dis_m_t)   0.0f,
        .y_m          = (odometry_dis_m_t)   0.0f,
        .theta_rad    = (odometry_ang_rad_t) 0.0f,
        .phi_l_rad    = odometry_normalize_angle(encoder_read_angle(ENCODER_L)),
        .phi_r_rad    = odometry_normalize_angle(encoder_read_angle(ENCODER_R)),
        .v_mps        = (odometry_vel_mps_t) 0.0f,
        .omega_rps    = (odometry_vel_rps_t) 0.0f,
        .last_time_us = esp_timer_get_time()
    };
}

odometry_ang_rad_t odometry_normalize_angle(odometry_ang_rad_t angle) {
    while (angle > +M_PI) angle -= 2.0f * M_PI;
    while (angle < -M_PI) angle += 2.0f * M_PI;

    return angle;
}

void odometry_step() {
    int64_t now_us = esp_timer_get_time();

    int64_t dt_us = now_us - odometry.last_time_us;
    if (dt_us <= ODOMETRY_PERIOD_MIN_US) {
        return;
    }
    float dt_s = ((float) dt_us) / 1000000.0f;
    odometry.last_time_us = now_us;

    odometry_ang_rad_t phi_l_rad = odometry_normalize_angle(
        encoder_read_angle(ENCODER_L)
    );
    odometry_ang_rad_t phi_r_rad = odometry_normalize_angle(
        encoder_read_angle(ENCODER_R)
    );

    odometry_ang_rad_t delta_phi_l_rad = odometry_normalize_angle(
        phi_l_rad - odometry.phi_l_rad
    );
    odometry_ang_rad_t delta_phi_r_rad = odometry_normalize_angle(
        phi_r_rad - odometry.phi_r_rad
    );

    odometry_dis_m_t dist_l_m = delta_phi_l_rad * CONFIG_WHEEL_RADIUS_M;
    odometry_dis_m_t dist_r_m = delta_phi_r_rad * CONFIG_WHEEL_RADIUS_M;

    odometry_dis_m_t   delta_dist_m    = (dist_r_m + dist_l_m) / 2.0f;
    odometry_ang_rad_t delta_theta_rad = (
        (dist_r_m - dist_l_m) / CONFIG_WHEELBASE_M
    );

    odometry_ang_rad_t alpha_rad = odometry.theta_rad + delta_theta_rad / 2.0f;
    odometry_ang_rad_t theta_rad = odometry.theta_rad + delta_theta_rad;

    odometry.x_m      += delta_dist_m * cosf(alpha_rad);
    odometry.y_m      += delta_dist_m * sinf(alpha_rad);
    odometry.theta_rad = odometry_normalize_angle(theta_rad);
    odometry.phi_r_rad = phi_r_rad;
    odometry.phi_l_rad = phi_l_rad;
    odometry.v_mps     = delta_dist_m / dt_s;
    odometry.omega_rps = delta_theta_rad / dt_s;

    LOG_D(
        "(x: %.02f m, y:%.02f m, theta: %.04f rad, v: %.02f m/s, omega: %.04f rad/s)",
        (float) odometry.x_m,
        (float) odometry.y_m,
        (float) odometry.theta_rad,
        (float) odometry.v_mps,
        (float) odometry.omega_rps
    );
}
