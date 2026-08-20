/**
 * @file countdown.h
 * @brief `STATE_COUNTDOWN` definition of the FSM callback functions `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#define COUNTDOWN_MS 5000

/**
 * @brief Run handler for @ref STATE_COUNTDOWN.
 *
 * Transitions to @ref STATE_SEARCH after a 5-second delay from state entry.
 *
 * @note If the Radio Controller is disconnected, transitions to @ref STATE_SAFE
 * resetting the countdown.
 */
void countdown_run(void);
