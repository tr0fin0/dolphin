/**
 * @file safe.h
 * @brief ``STATE_SAFE`` definition of the FSM callback functions
 * ``on_entry()``, and ``on_run()``.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Entry handler for ``STATE_SAFE``.
 *
 * Calls ``controller_stop()``.
 */
void safe_entry(void);

/**
 * @brief Run handler for ``STATE_SAFE``.
 *
 * Keeps system in a safe to manipulate state.
 *
 * @note Transition to ``STATE_AUTONOMOUS`` when:
 * - ``CONFIG_CONTROL_AUTONOMOUS``
 *
 * - ``OPENING_STATE_FINISHED``
 *
 * - ``IR_STATE_START``
 *
 * @note Transition to ``STATE_MANUAL`` when:
 * - ``CONFIG_CONTROL_RADIO``
 *
 * - ``OPENING_STATE_FINISHED``
 *
 * - ``RADIO_STATUS_CONNECTED``
 *
 * @note Transition to ``STATE_OPENING`` when:
 * - ``CONFIG_CONTROL_AUTONOMOUS``
 *
 *   - **not** ``OPENING_STATE_FINISHED``
 *
 *   - ``RADIO_STATUS_CONNECTED``
 *
 *   - ``IR_STATE_STANDBY``
 *
 * - ``CONFIG_CONTROL_RADIO``
 *
 *   - **not** ``OPENING_STATE_FINISHED``
 *
 *   - ``RADIO_STATUS_CONNECTED``
 */
void safe_run(void);
