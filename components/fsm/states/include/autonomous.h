/**
 * @file autonomous.h
 * @brief STATE_AUTONOMOUS definition of the FSM callback functions `on_exit()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-22
 */

#pragma once

/**
 * @brief Exit handler for @ref STATE_AUTONOMOUS.
 *
 */
void autonomous_exit(void);

/**
 * @brief Run handler for @ref STATE_AUTONOMOUS.
 *
 * @note
 * - Transition to @ref STATE_ATTACK if the adversary is aligned with the front.
 *
 * - Transition to @ref STATE_SEARCH if the adversary is lost.
 *
 * - Transition to @ref STATE_SAFE if the IR receiver is at @ref IR_STATE_STOP.
 *
 * - Transition to @ref STATE_SURVIVE if the dojo edge is detected.
 */
void autonomous_run(void);