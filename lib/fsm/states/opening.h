/**
 * @file opening.h
 * @brief `STATE_OPENING` definition of the FSM callback functions `on_entry()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
 * @brief Opening state entry handler.
 *
 * Sets visual indication upon entering `STATE_OPENING`.
 */
void opening_entry(void);

/**
 * @brief Opening state run handler.
 *
 * Reads throttle value in 3 sequencial steps to determine the opening move
 * requested among the defined below:
 */
void opening_run(void);
