/**
 * @file attack.h
 * @brief ``STATE_ATTACK`` definition of the FSM callback functions
 * ``on_entry()``, ``on_exit()``, and ``on_run()``.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Entry handler for ``STATE_ATTACK``.
 *
 * Calls ``controller_stop()``.
 */
void attack_entry(void);

/**
 * @brief Exit handler for ``STATE_ATTACK``.
 *
 * Calls ``controller_stop()``.
 */
void attack_exit(void);

/**
 * @brief Run handler for ``STATE_ATTACK``.
 *
 * While the ``ir_t`` is ``IR_STATE_START``, autonomously push the adversary.
 *
 * @note Transition to ``STATE_SAFE`` when:
 * - **not** ``IR_STATE_START``
 *
 * @note Transition to ``STATE_SURVIVE`` when:
 * - ``sensor_detected_line()``
 *
 * @note Transition to ``STATE_AUTONOMOUS`` when:
 * - **not** ``sensor_detected_obstacle_front()``
 */
void attack_run(void);
