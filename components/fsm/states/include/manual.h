/**
 * @file manual.h
 * @brief ``FSM_STATE_MANUAL`` definition of the FSM callback functions
 * ``on_exit()``, and ``on_run()``.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Exit handler for ``FSM_STATE_MANUAL``.
 *
 * Calls ``esc_set_pwm_mix_neutral()``.
 */
void manual_exit(void);

/**
 * @brief Run handler for ``FSM_STATE_MANUAL``.
 *
 * While the ``radio`` is ``RADIO_STATUS_CONNECTED``, reads the steering and
 * throttle channels and forwards them to the ``esc``.
 *
 * @note Transition to ``FSM_STATE_SAFE`` when:
 * - ``RADIO_STATUS_DISCONNECTED``
 */
void manual_run(void);
