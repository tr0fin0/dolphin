/**
 * @file search.h
 * @brief `STATE_SEARCH` definition of the FSM callback functions `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Run handler for @ref STATE_SEARCH.
 *
 * While the Raio Controller is connected, autonomous align with the adversary.
 *
 * @note
 * - Transition to @ref STATE_ATTACK if the adversary is aligned with the front.
 *
 * - Transition to @ref STATE_SAFE if the Radio Controller is disconnected.
 *
 * - Transition to @ref STATE_SURVIVE if the dojo edge is detected.
 */
void search_run(void);
