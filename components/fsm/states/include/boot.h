/**
 * @file boot.h
 * @brief ``FSM_STATE_BOOT`` definition of the FSM callback functions
 * ``on_run()``.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Run handler for ``FSM_STATE_BOOT``.
 *
 * Used as the ``fsm`` entry point.
 *
 * @note Transition to ``FSM_STATE_BOOT`` unconditionally.
 */
void boot_run(void);
