/**
 * @file survive.h
 * @brief STATE_SURVIVE definition of the FSM callback functions ``on_entry()``
 * , ``on_exit()`` and ``on_run()``.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#include "controller.h"
#include "sensor.h"

/**
 * @brief
 */
typedef enum survive_move {
    SURVIVE_MOVE_STATIC = 0,    /**< . */
    SURVIVE_MOVE_B,             /**< . */
    SURVIVE_MOVE_BL,            /**< . */
    SURVIVE_MOVE_BR,            /**< . */
    SURVIVE_MOVE_F,             /**< . */
    SURVIVE_MOVE_FL,            /**< . */
    SURVIVE_MOVE_FR,            /**< . */
    SURVIVE_MOVE_L,             /**< . */
    SURVIVE_MOVE_R,             /**< . */
    NUMBER_OF_SURVIVE_MOVES     /**< . */
} survive_move_t;
 
/**
 * @brief
 */
typedef struct survive_config {
    const char *name;                           /**< . */
    const controller_sequence_t *sequence;      /**< . */
    float sensors_states[NUMBER_OF_SENSORS];    /**< . */
} survive_config_t;

/**
 * @brief
 */
typedef struct survive {
    const char *name;                                   /**< . */
    bool sensors_active[NUMBER_OF_SENSORS];             /**< . */
    float sensors_values[NUMBER_OF_SENSORS];            /**< . */
    survive_move_t move;                                /**< . */
    survive_config_t moves[NUMBER_OF_SURVIVE_MOVES];    /**< . */
} survive_t;

/**
 * @brief Entry handler for @ref STATE_SURVIVE.
 *
 * Calls @ref controller_stop .
 */
void survive_entry(void);

/**
 * @brief Exit handler for @ref STATE_SURVIVE.
 *
 * Calls @ref controller_stop .
 */
void survive_exit(void);

/**
 * @brief Returns the survive move name.
 *
 * @return Human-readable null-terminated string representing the move name.
 * @retval NULL If strategy is invalid.
 */
const char *survive_get_move_name(survive_move_t move);

/**
 * @brief Run handler for @ref STATE_SURVIVE.
 *
 * While the Radio Controller is connected, autonomous avoid leaving the dojo.
 *
 * @note
 * - Transition to @ref STATE_ATTACK if the adversary is aligned with the front.
 *
 * - Transition to @ref STATE_SAFE if the Radio Controller is disconnected.
 *
 * - Transition to @ref STATE_SEARCH if the adversary is lost.
 */
void survive_run(void);
