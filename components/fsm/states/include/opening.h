/**
 * @file opening.h
 * @brief STATE_OPENING definition of the FSM callback functions `on_entry()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#include "pwm.h"

/**
 * @brief Opening Finite State Machine states.
 */
typedef enum opening_state {
    OPENING_STATE_EXECUTION = 0,    /**< In either @ref config_control_mode_t , opening strategy is executed. */
    OPENING_STATE_FINISHED,         /**< In either @ref config_control_mode_t , opening strategy execution is finished. */
    OPENING_STATE_RELEASE,          /**< In either @ref config_control_mode_t , opening strategy execution is waiting release commad. */
    OPENING_STATE_SELECTION,        /**< In either @ref config_control_mode_t , opening strategy is selected. */
    NUMBER_OF_OPENING_STATES,       /**< Number of opening FSM states. */
} opening_state_t;

/**
 * @brief Radio Controlled possible opening moves.
 *
 * Each digit from left to right represents the measure throttle value at the
 * respective iteration.
 *
 * @note An opening strategy of value ``321`` is obtained by having a throttle
 * channel value of ``3`` in the first measure, ``2`` in the second measure and
 * ``1`` in the third measure.
 */
typedef enum opening {
    OPENING_STATIC = 0, /**< ``222``: Remains in the initial position and rotation. */
    OPENING_DRAW,       /**< ``221``: Remains in the initial position and rotate 180 degrees. */
    OPENING_NE,         /**< ``331``: Goes to north-east. */
    OPENING_NN,         /**< ``232``: Goes to north. */
    OPENING_NW,         /**< ``133``: Goes to north-west. */
    OPENING_SEN,        /**< ``312``: Goes to south-east with neutral rotation. */
    OPENING_SE,         /**< ``313``: Goes to south-east. */
    OPENING_SS,         /**< ``212``: Goes to south. */
    OPENING_SW,         /**< ``111``: Goes to south-west. */
    OPENING_SWN,        /**< ``112``: Goes to south-west with neutral rotation. */
    NUMBER_OF_OPENINGS  /**< Number of openings positions. */
} opening_t;

/**
 * @brief Opening strategy code.
 *
 * Each code is composed by 3 digits where individual digit values may be:
 *
 * - `3` if radio receiver channel is `> (PWM_NEUTRAL_US + PWM_MAXIMUM_US) / 2`.
 *
 * - `1` if radio receiver channel is `< (PWM_NEUTRAL_US + PWM_MINIMUM_US) / 2`.
 *
 * - `2` if radio receiver channel is not in the previous intervals.
 */
typedef uint16_t opening_code_t;

/**
 * @brief Opening iteration step.
 */
typedef uint8_t opening_step_t;

/**
 * @brief Opening configuration.
 */
typedef struct opening_config {
    const char *name;               /**< Human-readable null-terminated Opening name. */
    const opening_code_t code;      /**< Opening code. */
} opening_config_t;

/**
 * @brief Opening strategy handler.
 */
typedef struct opening_handler {
    const char *name;                                   /**< Human-readable null-terminated opening handler name. */
    opening_t strategy;                                 /**< Opening handler strategy selected. */
    opening_config_t strategies[NUMBER_OF_OPENINGS];    /**< Opening handler strategies configurations. */
    opening_code_t code;                                /**< Opening handler strategy selected code. */
    opening_step_t step;                                /**< Opening handler strategy selection step. */
    opening_state_t state;                              /**< Opening handler state. */
    const char *states_names[NUMBER_OF_OPENING_STATES]; /**< Human-readable null-terminated opening handler states names. */
    pwm_norm_t last_button;                             /**< Opening handler last button measure. */
} opening_handler_t;

/**
 * @def OPENING_ITERATIONS
 * @brief Quantity of iterations required to select the opening strategy when in
 * the @ref CONFIG_CONTROL_RADIO.
 *
 * **Default Value:** 3
 */
#define OPENING_ITERATIONS 3

/**
 * @brief Entry handler for @ref STATE_OPENING.
 *
 * Set @ref LED_STATE to @ref LED_COLOR_PURPLE and captures current Radio Controller
 * button value.
 */
void opening_entry(void);

/**
 * @brief 
 */
opening_state_t opening_get_status(void);

/**
 * @brief Run handler for @ref STATE_OPENING.
 *
 * While Radio Controller is connected, 3 sequential reads of the throttle
 * channel are used to determine the opening move.
 */
void opening_run(void);
