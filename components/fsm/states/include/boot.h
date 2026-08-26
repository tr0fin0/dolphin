/**
 * @file boot.h
 * @brief STATE_BOOT definition of the FSM callback functions `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Run handler for @ref STATE_BOOT.
 *
 * Unconditionally transitions to @ref STATE_SAFE.
 *
 * @note @ref STATE_BOOT is used as the FSM entry point.
 */
void boot_run(void);
