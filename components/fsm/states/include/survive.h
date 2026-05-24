/**
 * @file survive.h
 * @brief `STATE_SURVIVE` definition of the FSM callback functions `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Run handler for `STATE_SURVIVE`.
 *
 * While the Radio Controller is connected, autonomous avoid leaving the dojo.
 *
 * @note
 * - Transition to `STATE_ATTACK` if the adversary is aligned with the front.
 *
 * - Transition to `STATE_SAFE` if the Radio Controller is disconnected.
 *
 * - Transition to `STATE_SEARCH` if the adversary is lost.
 */
void survive_run(void);
