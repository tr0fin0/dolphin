/**
 * @file countdown.h
 * @brief `STATE_COUNTDOWN` definition of the FSM callback functions
 * `on_entry()` and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

#define COUNTDOWN_MS 5000

/**
 * @brief Countdown state entry handler.
 *
 * Sets visual indication for countdown phase and captures the entry timestamp
 * for time-based transition.
 */
void countdown_entry(void);

/**
 * @brief Countdown state run handler.
 *
 * Transitions to `STATE_SEARCH` after a 5-second delay from state entry. If the
 * radio enable signal is lost, immediately transitions back to `STATE_SAFE`.
 */
void countdown_run(void);
