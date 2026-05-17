/**
 * @file attack.h
 * @brief `STATE_ATTACK` definition of the FSM callback functions `on_entry()`,
 * `on_exit()` and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Run handler for `STATE_ATTACK`.
 *
 * While the Radio Controller is connected, autonomously push the adversary.
 *
 * @note
 * - Transition to `STATE_SEARCH` if the adversary is lost.
 *
 * - Transition to `STATE_SAFE` if the Radio Controller is disconnected.
 *
 * - Transition to `STATE_SURVIVE` if the dojo edge is detected.
 */
void attack_run(void);

/**
 * @brief Exit handler for `STATE_ATTACK`.
 *
 * Ensures both motors are driven to neutral PWM, preventing unintended motion
 * during transition.
 */
void attack_exit(void);
