/**
 * @file controller.h
 * @brief Waypoint navigation controller for a differential drive robot.
 *
 * Dual PID controllers are used to navigate from the current odometry state to
 * a target odometry waypoint.
 *
 * @note Navigation operates in two phases:
 *
 * 1. Approach: Drives towards the target (x, y) coordinates.
 *
 * 2. Alignment: Rotates in place to match the target's final heading (theta).
 *
 * @author Guilherme Nunes Trofino
 * @date 2025-07-28
 */

#include "odometry.h"

/**
 * @def CONTROLLER_V_MAX_MPS
 * @brief Controller maximum linear velocity in meters per second.
 *
 * **Default Value:** 1.5
 */
#define CONTROLLER_V_MAX_MPS            1.5f

/**
 * @def CONTROLLER_PID_ANGULAR_KP
 * @brief Controller PID angular proportional constant value.
 *
 * **Default Value:** 1.0
 */
#define CONTROLLER_PID_ANGULAR_KP       1.0f

/**
 * @def CONTROLLER_PID_ANGULAR_KI
 * @brief Controller PID angular integrative constant value.
 *
 * **Default Value:** 1.0
 */
#define CONTROLLER_PID_ANGULAR_KI       1.0f

/**
 * @def CONTROLLER_PID_ANGULAR_KD
 * @brief Controller PID angular derivative constant value.
 *
 * **Default Value:** 1.0
 */
#define CONTROLLER_PID_ANGULAR_KD       1.0f

/**
 * @def CONTROLLER_PID_ANGULAR_I_MAX
 * @brief Controller PID angular maximum integrative error value.
 *
 * **Default Value:** 100.0
 */
#define CONTROLLER_PID_ANGULAR_I_MAX  100.0f

/**
 * @def CONTROLLER_PID_LINEAR_KP
 * @brief Controller PID linear proportional constant value.
 *
 * **Default Value:** 1.0
 */
#define CONTROLLER_PID_LINEAR_KP        1.0f

/**
 * @def CONTROLLER_PID_LINEAR_KI
 * @brief Controller PID linear integrative constant value.
 *
 * **Default Value:** 1.0
 */
#define CONTROLLER_PID_LINEAR_KI        1.0f

/**
 * @def CONTROLLER_PID_LINEAR_KD
 * @brief Controller PID linear derivative constant value.
 *
 * **Default Value:** 1.0
 */
#define CONTROLLER_PID_LINEAR_KD        1.0f

/**
 * @def CONTROLLER_PID_LINEAR_I_MAX
 * @brief Controller PID linear maximum integrative error.
 *
 * **Default Value:** 100.0
 */
#define CONTROLLER_PID_LINEAR_I_MAX   100.0f

/**
 * @brief Initializes the angular and linear PID controllers errors and times.
 */
void controller_init(void);

/**
 * @brief Controls navigation via the ESCs PWMs to reach the target waypoint.
 *
 * Computes the necessary linear and angular velocities within maximum limits
 * and applies differential drive inverse kinematics to command the ESCs.
 *
 * @note Calls @ref esc_set_pwm_mix_neutral upon reaching the target waypoint.
 *
 * @param[in] current The current waypoint.
 * @param[in] target The desired waypoint.
 *
 * @return
 * - `true` if the target waypoint is not yet reached.
 *
 * - `false` if the target waypoint is reached within tolerances.
 */
bool controller_navigate(odometry_waypoint_t current, odometry_waypoint_t target);
