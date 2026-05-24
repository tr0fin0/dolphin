/**
 * @file manual.h
 * @brief `STATE_MANUAL` definition of the FSM callback functions `on_exit()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Run handler for `STATE_MANUAL`.
 *
 * While the Radio Controller is connected, reads the steering and throttle
 * channels and forwards them to the motor controllers.
 *
 * @note Transition to `STATE_SAFE` if the Radio Controller is disconnected.
 */
void manual_run(void);

/**
 * @brief Exit handler for `STATE_MANUAL`.
 *
 * Ensures both motors are driven to neutral PWM, preventing unintended motion
 * during transition.
 */
void manual_exit(void);
