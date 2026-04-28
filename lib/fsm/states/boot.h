/**
 * @file boot.h
 * @brief `STATE_BOOT` definition of the FSM callback functions `on_entry()` and
 * `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
 * @brief Entry handler for `STATE_BOOT`.
 *
 * Set `LED_STATE` to `COLOR_WHITE`.
 */
void boot_entry(void);

/**
 * @brief Run handler for `STATE_BOOT`.
 *
 * Unconditionally transitions to `STATE_SAFE`.
 *
 * @note `STATE_BOOT` is used as the FSM entry point.
 */
void boot_run(void);
