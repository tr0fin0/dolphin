/**
 * @file attack.h
 * @brief `STATE_ATTACK` definition of the FSM callback functions `on_exit()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Run handler for @ref STATE_ATTACK.
 *
 * While the Radio Controller is connected, autonomously push the adversary.
 *
 * @note
 * - Transition to @ref STATE_SEARCH if the adversary is lost.
 *
 * - Transition to @ref STATE_SAFE if the Radio Controller is disconnected.
 *
 * - Transition to @ref STATE_SURVIVE if the dojo edge is detected.
 */
void attack_run(void);

/**
 * @brief Exit handler for @ref STATE_ATTACK.
 *
 * Ensures both motors are driven to @ref PWM_NEUTRAL_US, preventing unintended
 * motion during transition.
 */
void attack_exit(void);
