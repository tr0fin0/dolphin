/**
 * @file attack.h
 * @brief `STATE_ATTACK` definition of the FSM callback functions `on_entry()`,
 * `on_exit()` and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
 * @brief Attack state entry handler.
 *
 * Sets visual indication upon entering `STATE_ATTACK`.
 */
void attack_entry(void);

/**
 * @brief Attack state run handler.
 *
 * Monitors radio enable signal and transitions to `STATE_SAFE` if control
 * authorization is lost.
 */
void attack_run(void);
