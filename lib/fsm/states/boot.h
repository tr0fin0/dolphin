/**
 * @file boot.h
 * @brief `STATE_BOOT` definition of the FSM callback functions `on_entry()` and
 * `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
 * @brief Boot state entry handler.
 *
 * Sets visual indication upon entering `STATE_BOOT`.
 */
void boot_entry(void);

/**
 * @brief Boot state run handler.
 *
 * First .
 */
void boot_run(void);
