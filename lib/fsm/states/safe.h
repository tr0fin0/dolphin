/**
 * @file safe.h
 * @brief `STATE_SAFE` definition of the FSM callback functions `on_entry()` and
 * `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
 * @brief Safe state entry handler.
 *
 * Activates visual warning indication and ensures all motors are stopped upon
 * entering the safe state.
 */
void safe_entry(void);

/**
 * @brief Safe state run handler.
 *
 * Monitors radio status and transitions to `STATE_COUNTDOWN`, `STATE_MANUAL` or
 * `STATE_OPENING` when a valid enable signal is detected.
 */
void safe_run(void);
