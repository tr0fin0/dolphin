/**
 * @file survive.h
 * @brief `STATE_SURVIVE` definition of the FSM callback functions `on_entry()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
 * @brief Survive state entry handler.
 *
 * Sets visual indication once upon entering `STATE_SURVIVE` and captures the
 * entry timestamp for time-based logic.
 */
void survive_entry(void);

/**
 * @brief Survive state run handler.
 *
 * Maintains safety monitoring and executes a timed maneuver:
 *
 * - Immediately transitions to `STATE_SAFE` if radio enable is lost.
 *
 * - After 500 ms from state entry, transitions to `STATE_SEARCH`.
 */
void survive_run(void);
