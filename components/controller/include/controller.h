/**
 * @file controller.h
 * @brief Waypoint navigation and kinematics controller for a differential drive robot.
 *
 * Dual PID controllers are used to navigate from the current odometry state to
 * a target waypoint.
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

#define CONTROLLER_V_MAX_MPS            1.5f
#define CONTROLLER_PID_ANGULAR_KP       1.0f
#define CONTROLLER_PID_ANGULAR_KI       1.0f
#define CONTROLLER_PID_ANGULAR_KD       1.0f
#define CONTROLLER_PID_ANGULAR_I_MAX  100.0f
#define CONTROLLER_PID_LINEAR_KP        1.0f
#define CONTROLLER_PID_LINEAR_KI        1.0f
#define CONTROLLER_PID_LINEAR_KD        1.0f
#define CONTROLLER_PID_LINEAR_I_MAX   100.0f

/**
 * @brief Initializes the angular and linear PID controllers with default gains.
 */
void controller_init(void);

/**
 * @brief Computes navigation velocities and set ESCs PWMs to reach a target.
 *
 * Calculates the Euclidean distance and angular errors between the current
 * and the target waypoint. Then computes the necessary linear and angular
 * velocities within maximum limits and applies differential drive inverse
 * kinematics to command the ESCs.
 *
 * @note `esc_set_mix_neutral()` is called upon reaching the target waypoint.
 *
 * @param current The robot's current odometry state.
 * @param target The desired target waypoint constraints.
 *
 * @return
 *
 * - `true` if the robot is actively navigating to the waypoint.
 *
 * - `false` if the robot has arrived within the target's tolerances.
 */
bool controller_navigate(odometry_waypoint_t current, odometry_waypoint_t target);
