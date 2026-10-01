/**
 * @file autonomous.h
 * @brief ``STATE_AUTONOMOUS`` definition of the FSM callback functions
 * ``on_entry()``, ``on_exit()``, and ``on_run()``.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-22
 */

#pragma once

#include "controller.h"
#include <stdint.h>

/**
 * @brief Autonous state handler.
 */
typedef struct autonomous_handler {
    const char *name;                       /**< Human-readable, null-terminated name of the autonomous handler. */
    const controller_sequence_t *movement;  /**< Minimal movement sequence to avoid static condition. */
    int64_t interval_us;                    /**< Maximum interval between each movement in microseconds. */
    int64_t last_time_us;                   /**< Last movement time in microseconds. */
} autonomous_handler_t;

/**
 * @brief Entry handler for ``STATE_AUTONOMOUS``.
 *
 * Calls ``controller_stop()`` and initialize time measurements.
 */
void autonomous_entry(void);

/**
 * @brief Exit handler for ``STATE_AUTONOMOUS``.
 *
 * Calls ``controller_stop()`` .
 */
void autonomous_exit(void);

/**
 * @brief Run handler for ``STATE_AUTONOMOUS``.
 *
 * While the ``ir_t`` is ``IR_STATE_START``, performs a minimal movement
 * sequence to avoid static condition.
 *
 * @note Transition to ``STATE_SAFE`` when:
 * - **not** ``IR_STATE_START``
 *
 * @note Transition to ``STATE_SURVIVE`` when:
 * - ``sensor_detected_line()``
 *
 * @note Transition to ``STATE_ATTACK`` when:
 * - ``sensor_detected_obstacle_front()``
 *
 * @note Transition to ``STATE_SEARCH`` when:
 * - ``sensor_detected_obstacle_sides()``
 */
void autonomous_run(void);
