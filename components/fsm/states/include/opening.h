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
 * @brief Opening strategies.
 *
 * Each strategy has an unique @ref opening_code_t identifier as presented
 * below with it's brief description.
 */
typedef enum opening {
    /**
     * @brief Remains in the initial position and rotation.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_N
     *
     * - 2: @ref OPENING_CODE_N
     *
     * - 3: @ref OPENING_CODE_N
     */
    OPENING_STATIC = 0,
    /**
     * @brief Remains in the initial position and rotate 180 degrees.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_L
     *
     * - 2: @ref OPENING_CODE_N
     *
     * - 3: @ref OPENING_CODE_N
     */
    OPENING_DRAW,
    /**
     * @brief Goes to north-east position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_L
     *
     * - 2: @ref OPENING_CODE_H
     *
     * - 3: @ref OPENING_CODE_H
     */
    OPENING_NE,
    /**
     * @brief Goes to north position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_N
     *
     * - 2: @ref OPENING_CODE_H
     *
     * - 3: @ref OPENING_CODE_N
     */
    OPENING_NN,
    /**
     * @brief Goes to north-west position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_H
     *
     * - 2: @ref OPENING_CODE_H
     *
     * - 3: @ref OPENING_CODE_L
     */
    OPENING_NW,
    /**
     * @brief Goes to south-east with neutral rotation position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_N
     *
     * - 2: @ref OPENING_CODE_L
     *
     * - 3: @ref OPENING_CODE_H
     */
    OPENING_SEN,
    /**
     * @brief Goes to south-east position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_H
     *
     * - 2: @ref OPENING_CODE_L
     *
     * - 3: @ref OPENING_CODE_H
     */
    OPENING_SE,
    /**
     * @brief Goes to south position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_N
     *
     * - 2: @ref OPENING_CODE_L
     *
     * - 3: @ref OPENING_CODE_N
     */
    OPENING_SS,
    /**
     * @brief Goes to south-west position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_L
     *
     * - 2: @ref OPENING_CODE_L
     *
     * - 3: @ref OPENING_CODE_L
     */
    OPENING_SW,
    /**
     * @brief Goes to south-west with neutral rotation position.
     *
     * Obtained when in each step:
     *
     * - 1: @ref OPENING_CODE_N
     *
     * - 2: @ref OPENING_CODE_L
     *
     * - 3: @ref OPENING_CODE_L
     */
    OPENING_SWN,
    NUMBER_OF_OPENINGS  /**< Number of openings positions. */
} opening_t;

/**
 * @brief Opening strategy code.
 *
 * Uses @ref radio_t signals from 2 channels to determine the 3 digit opening
 * code where each individual digit value may be:
 *
 * - @ref OPENING_CODE_H
 *
 * - @ref OPENING_CODE_L
 *
 * - @ref OPENING_CODE_N
 *
 * The @ref radio_t channel value is measured when the signal another channel
 * changes. Typically, the throttle channel value is measured when the button
 * channel is pressed.
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
 * @def OPENING_CODE_H
 * @brief Opening code digit representing @ref pwm_norm_t high value.
 *
 * If radio receiver channel is ``> ( PWM_NEUTRAL_US + PWM_MAXIMUM_US ) / 2``.
 *
 * **Default Value:** 3
 */
#define OPENING_CODE_H 3

/**
 * @def OPENING_CODE_L
 * @brief Opening code digit representing @ref pwm_norm_t low value.
 *
 * If radio receiver channel is ``< ( PWM_NEUTRAL_US + PWM_MINIMUM_US ) / 2``.
 *
 * **Default Value:** 1
 */
#define OPENING_CODE_L 1

/**
 * @def OPENING_CODE_N
 * @brief Opening code digit representing @ref pwm_norm_t neutral value.
 *
 * If radio receiver channel is between:
 *
 * - ``( PWM_NEUTRAL_US + PWM_MINIMUM_US ) / 2``
 *
 * - ``( PWM_NEUTRAL_US + PWM_MAXIMUM_US ) / 2``
 *
 * **Default Value:** 2
 */
#define OPENING_CODE_N 2

/**
 * @def OPENING_INITIAL_BUTTON
 * @brief Initial button value to select the opening strategy.
 *
 * **Default Value:** 0
 */
#define OPENING_INITIAL_BUTTON 0

/**
 * @def OPENING_INITIAL_CODE
 * @brief Initial code value to select the opening strategy.
 *
 * **Default Value:** 0
 */
#define OPENING_INITIAL_CODE 0

/**
 * @def OPENING_INITIAL_ITERATION
 * @brief Initial iteration value to select the opening strategy.
 *
 * **Default Value:** 0
 */
#define OPENING_INITIAL_ITERATION 0

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
