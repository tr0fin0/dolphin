/**
 * @file search.h
 * @brief ``FSM_STATE_SEARCH`` definition of the FSM callback functions
 * ``on_entry()``, ``on_exit()``, and ``on_run()``
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Entry handler for ``FSM_STATE_SEARCH``.
 *
 * Calls ``controller_stop()``.
 */
void search_entry(void);

/**
 * @brief Exit handler for ``FSM_STATE_SEARCH``.
 *
 * Calls ``controller_stop()``.
 */
void search_exit(void);

/**
 * @brief Run handler for ``FSM_STATE_SEARCH``.
 *
 * While the ``radio`` is ``RADIO_STATUS_CONNECTED``, autonomous align with the
 * adversary.
 *
 * @note Transition to ``FSM_STATE_SAFE`` when:
 * - **not** ``IR_STATE_START``
 *
 * @note Transition to ``FSM_STATE_SURVIVE`` when:
 * - ``sensor_detected_line()``
 *
 * @note Transition to ``FSM_STATE_AUTONOMOUS`` when:
 * - **not** ``sensor_detected_obstacle_sides()``
 */
void search_run(void);
