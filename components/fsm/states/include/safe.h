/**
 * @file safe.h
 * @brief `STATE_SAFE` definition of the FSM callback functions `on_entry()` and
 * `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Entry handler for `STATE_SAFE`.
 *
 * Set `LED_STATE` to `COLOR_RED` and ensures both motors are driven to neutral
 * PWM.
 */
void safe_entry(void);

/**
 * @brief Run handler for `STATE_SAFE`.
 *
 * When Radio Controller is connected, transition to `STATE_COUNTDOWN` if in
 * autonomous control and to `STATE_MANUAL` or `STATE_OPENING` if in radio
 * control.
 *
 * @note
 * - If Radio Controller is disconnected during opening selection,
 * `opening_step` remains unchanged.
 *
 * - Once an opening is executed, the `STATE_OPENING` stays unreachable
 * until system reset.
 */
void safe_run(void);
