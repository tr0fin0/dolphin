/**
 * @file survive.h
 * @brief ``FSM_STATE_SURVIVE`` definition of the FSM callback functions
 * ``on_entry()``, ``on_exit()``, and ``on_run()``.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#include "controller.h"
#include "sensor.h"

/**
 * @brief Survive moves.
 *
 * Each move is triggered by an unique combination of sensors states as
 * describled below.
 */
typedef enum survive_move {
    /**
     * @brief Maintains the current position and rotation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MAX_VALUE``
     */
    SURVIVE_MOVE_STATIC = 0,
    /**
     * @brief Moves forwards with a translation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MIN_VALUE``
     */
    SURVIVE_MOVE_B,
    /**
     * @brief Moves forwards with a translation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MIN_VALUE``
     */
    SURVIVE_MOVE_BL,
    /**
     * @brief Moves forwards with a translation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MIN_VALUE``
     */
    SURVIVE_MOVE_BR,
    /**
     * @brief Moves backwards with a translation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MAX_VALUE``
     */
    SURVIVE_MOVE_F,
    /**
     * @brief Moves backwards with a translation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MIN_VALUE``
     */
    SURVIVE_MOVE_FL,
    /**
     * @brief Moves backwards with a translation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MAX_VALUE``
     */
    SURVIVE_MOVE_FR,
    /**
     * @brief Moves clockwise with a rotation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MIN_VALUE``
     */
    SURVIVE_MOVE_L,
    /**
     * @brief Moves counter-clockwise with a rotation.
     *
     * Happens in the following sensor state:
     *
     * - ``SENSOR_QRE_BL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_BR``: ``SENSOR_MAX_VALUE``
     *
     * - ``SENSOR_QRE_FL``: ``SENSOR_MIN_VALUE``
     *
     * - ``SENSOR_QRE_FR``: ``SENSOR_MAX_VALUE``
     */
    SURVIVE_MOVE_R,
    NUMBER_OF_SURVIVE_MOVES     /**< Number of survive moves. */
} survive_move_t;

/**
 * @brief Configuration of a survive move.
 */
typedef struct survive_config {
    const char *name;                           /**< Human-readable, null-terminated name of the move. */
    const controller_sequence_t *sequence;      /**< Controller command sequence. */
    float sensors_states[NUMBER_OF_SENSORS];    /**< Survive move sensor states. */
} survive_config_t;

/**
 * @brief Survive state handler.
 */
typedef struct survive_handler {
    const char *name;                                   /**< Human-readable, null-terminated name of the survive handler. */
    bool sensors_active[NUMBER_OF_SENSORS];             /**< Array of sensors active considered for survive moves conditions. */
    float sensors_values[NUMBER_OF_SENSORS];            /**< Array of sensors values. */
    survive_move_t move;                                /**< Currently matched survive movement. */
    survive_config_t moves[NUMBER_OF_SURVIVE_MOVES];    /**< Configurations for all available survive movements. */
} survive_handler_t;

/**
 * @brief Entry handler for ``FSM_STATE_SURVIVE``.
 *
 * Calls ``controller_stop()``.
 */
void survive_entry(void);

/**
 * @brief Exit handler for ``FSM_STATE_SURVIVE``.
 *
 * Calls ``controller_stop()``.
 */
void survive_exit(void);

/**
 * @brief Returns the survive move name.
 *
 * @param[in] move Survive move.
 *
 * @return Human-readable null-terminated string representing the move name.
 * @retval NULL If strategy is invalid.
 */
const char *survive_get_move_name(survive_move_t move);

/**
 * @brief Run handler for ``FSM_STATE_SURVIVE``.
 *
 * While the ``ir_t`` is ``IR_STATE_START``, autonomous avoid leaving the dojo.
 *
 * @note Transition to ``FSM_STATE_SAFE`` when:
 * - **not** ``IR_STATE_START``
 *
 * @note Transition to ``FSM_STATE_AUTONOMOUS`` when:
 * - **not** ``sensor_detected_line()``
 */
void survive_run(void);
