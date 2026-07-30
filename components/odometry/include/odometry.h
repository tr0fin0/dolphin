/**
 * @file odometry.h
 * @brief Odometry for a differential driven architecture.
 *
 * @author Guilherme Nunes Trofino
 * @date 2025-07-28
 */

#pragma once

#include <math.h>
#include <stdint.h>

#define DEG2RAD(x) ((float) (((float) x) * M_PI / 180.0f))

#define ODOMETRY_PERIOD_MIN_US 1000

/**
 * @brief Odometry distance in meters.
 */
typedef float odometry_dis_m_t;

/**
 * @brief Odometry angle in radians between -PI and +PI.
 */
typedef float odometry_ang_rad_t;

/**
 * @brief Odometry linear velocity in meters per second.
 */
typedef float odometry_vel_mps_t;

/**
 * @brief Odometry angular velocity in radians per second.
 */
typedef float odometry_vel_rps_t;

/**
 * @brief Odometry measurements from a differential driven architecture.
 */
typedef struct odometry {
    odometry_dis_m_t   x_m;         /** Coordinate x integrated from origin in meters. */
    odometry_dis_m_t   y_m;         /** Coordinate y integrated from origin in meters. */
    odometry_ang_rad_t theta_rad;   /** Current normalized heading angle t in radians. */
    odometry_ang_rad_t phi_l_rad;   /** Current encoder angle in radians in the left motor. */
    odometry_ang_rad_t phi_r_rad;   /** Current encoder angle in radians in the right motor. */
    odometry_vel_mps_t v_mps;       /** Current linear velocity v in meters per second. */
    odometry_vel_rps_t omega_rps;   /** Current angular velocity w in radians per second. */
    int64_t last_time_us;
} odometry_t;

/**
 * @brief Odometry reference waypoint for navigation.
 */
typedef struct odometry_waypoint {
    odometry_dis_m_t   x_m;             /** Coordinate x from waypoint in meters. */
    odometry_dis_m_t   y_m;             /** Coordinate y from waypoint in meters. */
    odometry_ang_rad_t theta_rad;       /** Normalized heading angle t in radians. */
    odometry_vel_mps_t v_max_mps;       /** Maximum linear velocity v in meters per second. */
    odometry_vel_rps_t omega_max_rps;   /** Maximum angular velocity w in radians per second. */
    odometry_dis_m_t   d_err_m;         /** Acceptable distance error from waypoint coordinates (x, y) in meters. */
    odometry_ang_rad_t theta_err_rad;   /** Acceptable heading angle error from waypoint heading t in radians. */
} odometry_waypoint_t;

/**
 * @brief Returns the current odometry data.
 *
 * @param odometry Pointer to an `odometry_t`.
 */
void odometry_get(odometry_t *odometry);

/**
 * @brief Odometry initialization at the origin.
 *
 * @note Origin is considered at the center of the circle.
 */
void odometry_init(void);

/**
 * @brief Returns the angle normalized between [-PI, +PI] radians.
 *
 * @param angle Angle in radians.
 */
odometry_ang_rad_t odometry_normalize_angle(odometry_ang_rad_t angle);

/**
 * @brief Run one odometry step.
 */
void odometry_step(void);
