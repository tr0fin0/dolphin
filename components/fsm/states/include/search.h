/**
 * @file search.h
 * @brief ``STATE_SEARCH`` definition of the FSM callback functions
 * ``on_entry()``, ``on_exit()``, and ``on_run()``
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Entry handler for ``STATE_SEARCH``.
 *
 * Calls ``controller_stop()``.
 */
void search_entry(void);

/**
 * @brief Exit handler for ``STATE_SEARCH``.
 *
 * Calls ``controller_stop()``.
 */
void search_exit(void);

/**
 * @brief Run handler for ``STATE_SEARCH``.
 *
 * While the ``radio`` is ``RADIO_STATUS_CONNECTED``, autonomous align with the
 * adversary.
 *
 * @note Transition to ``STATE_SAFE`` when:
 * - **not** ``IR_STATE_START``
 *
 * @note Transition to ``STATE_SURVIVE`` when:
 * - ``sensor_detected_line()``
 *
 * @note Transition to ``STATE_AUTONOMOUS`` when:
 * - **not** ``sensor_detected_obstacle_sides()``
 */
void search_run(void);
