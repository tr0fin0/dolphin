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
        .x_m          = (float) 0.0f,
        .y_m          = (float) 0.0f,
        .theta_rad    = (float) 0.0f,
        .phi_l_rad    = odometry_normalize_angle(encoder_read_angle(ENCODER_L)),
        .phi_r_rad    = odometry_normalize_angle(encoder_read_angle(ENCODER_R)),
        .v_mps        = (float) 0.0f,
        .omega_rps    = (float) 0.0f,
        .last_time_us = esp_timer_get_time()
    };
}

float odometry_normalize_angle(float angle) {
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

    float phi_l_rad = odometry_normalize_angle(encoder_read_angle(ENCODER_L));
    float phi_r_rad = odometry_normalize_angle(encoder_read_angle(ENCODER_R));

    float delta_phi_l_rad = odometry_normalize_angle(
        phi_l_rad - odometry.phi_l_rad
    );
    float delta_phi_r_rad = odometry_normalize_angle(
        phi_r_rad - odometry.phi_r_rad
    );

    float dist_l_m = delta_phi_l_rad * CONFIG_WHEEL_RADIUS_M;
    float dist_r_m = delta_phi_r_rad * CONFIG_WHEEL_RADIUS_M;

    float delta_dist_m    = (dist_r_m + dist_l_m) / 2.0f;
    float delta_theta_rad = ((dist_r_m - dist_l_m) / CONFIG_WHEELBASE_M);

    float alpha_rad = odometry.theta_rad + delta_theta_rad / 2.0f;
    float theta_rad = odometry.theta_rad + delta_theta_rad;

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
