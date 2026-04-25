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
