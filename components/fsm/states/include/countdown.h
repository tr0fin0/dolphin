/**
 * @file countdown.h
 * @brief `STATE_COUNTDOWN` definition of the FSM callback functions
 * `on_entry()` and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#define COUNTDOWN_MS 5000

/**
 * @brief Entry handler for `STATE_COUNTDOWN`.
 *
 * Set `LED_STATE` to `COLOR_ORANGE_LIGHT` and captures current timestamp.
 */
void countdown_entry(void);

/**
 * @brief Run handler for `STATE_COUNTDOWN`.
 *
 * Transitions to `STATE_SEARCH` after a 5-second delay from state entry.
 *
 * @note If the Radio Controller is disconnected, transitions to `STATE_SAFE`
 * reseting the countdown.
 */
void countdown_run(void);
