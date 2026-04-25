#pragma once

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
