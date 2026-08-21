/**
 * @file safe.h
 * @brief @ref STATE_SAFE definition of the FSM callback functions `on_entry()` and
 * `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Entry handler for @ref STATE_SAFE.
 *
 * Set @ref LED_STATE to @ref LED_COLOR_RED and ensures both motors are driven
 * to @ref PWM_NEUTRAL_US.
 */
void safe_entry(void);

/**
 * @brief Run handler for @ref STATE_SAFE.
 *
 * When Radio Controller is connected, transition to @ref STATE_COUNTDOWN if in
 * @ref CONFIG_CONTROL_AUTONOMOUS and to @ref STATE_MANUAL or @ref STATE_OPENING
 * if in CONFIG_CONTROL_RADIO.
 *
 * @note
 * - If Radio Controller is disconnected during opening selection,
 * `opening_step` remains unchanged.
 *
 * - Once an opening is executed, the @ref STATE_OPENING stays unreachable
 * until system reset.
 */
void safe_run(void);
