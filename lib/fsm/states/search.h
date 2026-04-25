/**
 * @file search.h
 * @brief `STATE_SEARCH` definition of the FSM callback functions `on_entry()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
 * @brief Search state entry handler.
 *
 * Sets visual indication upon entering `STATE_SEARCH`.
 */
void search_entry(void);

/**
 * @brief Search state run handler.
 *
 * Continuously monitors radio status and transitions to `STATE_SAFE` if the
 * enable signal is lost.
 */
void search_run(void);
