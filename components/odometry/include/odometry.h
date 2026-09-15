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

/**
 * @def DEG2RAD(x)
 * @brief Converts an angle from degrees to radians.
 *
 * @param[in] x Angle in degrees between 0 and 360 degrees.
 * @return Angle in radians between 0 and 2PI radians.
 */
#define DEG2RAD(x) ((float) (((float) x) * M_PI / 180.0f))

/**
 * @def ODOMETRY_PERIOD_MIN_US
 * @brief Minimal Odometry update period in microseconds.
 *
 * **Default Value:** 1000
 */
#define ODOMETRY_PERIOD_MIN_US 1000

/**
 * @brief Odometry measurements from a differential driven architecture.
 */
typedef struct odometry {
    float x_m;              /**< Odometry coordinate ``x`` integrated from origin in meters. */
    float y_m;              /**< Odometry coordinate ``y`` integrated from origin in meters. */
    float theta_rad;        /**< Odometry normalized heading angle ``theta`` in radians. */
    float phi_l_rad;        /**< Odometry encoder angle ``phi_l`` in radians in the left motor. */
    float phi_r_rad;        /**< Odometry encoder angle ``phi_r`` in radians in the right motor. */
    float v_mps;            /**< Odometry linear velocity ``v`` in meters per second. */
    float omega_rps;        /**< Odometry angular velocity ``omega`` in radians per second. */
    int64_t last_time_us;   /**< Odometry last rising time in microseconds.*/
} odometry_t;

/**
 * @brief Odometry reference waypoint for navigation.
 */
typedef struct odometry_waypoint {
    float x_m;              /**< Waypoint coordinate ``x`` from origin in meters. */
    float y_m;              /**< Waypoint coordinate ``y`` from origin in meters. */
    float theta_rad;        /**< Waypoint normalized heading angle ``theta`` in radians. */
    float v_max_mps;        /**< Waypoint maximum linear velocity ``v`` in meters per second. */
    float omega_max_rps;    /**< Waypoint maximum angular velocity ``omega`` in radians per second. */
    float d_err_m;          /**< Waypoint acceptable distance error from coordinates ``(x, y)`` in meters. */
    float theta_err_rad;    /**< Waypoint acceptable heading angle error from angle ``theta`` in radians. */
} odometry_waypoint_t;

/**
 * @brief Returns the current odometry data.
 *
 * @param[in] odometry Pointer to an @ref odometry_t.
 */
void odometry_get(odometry_t *odometry);

/**
 * @brief Odometry initialization at the origin.
 *
 * @note Origin is considered at the center of the circle.
 */
void odometry_init(void);

/**
 * @brief Returns the angle normalized between -PI and +PI radians.
 *
 * @param[in] angle Angle in radians between 0 and 2PI.
 * @return Angle in radians between -PI and +PI.
 */
float odometry_normalize_angle(float angle);

/**
 * @brief Run one odometry step.
 */
void odometry_step(void);
