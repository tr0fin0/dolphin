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
 * Each strategy is identified by a unique sequence of @ref opening_code_t
 * values, as described below.
 */
typedef enum opening {
    /**
     * @brief Maintains the initial position and rotation.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_N
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_N
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_N
     */
    OPENING_STATIC = 0,
    /**
     * @brief Remains in the initial position and rotates 180 degrees.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_N
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_N
     */
    OPENING_DRAW,
    /**
     * @brief Moves to the north-east position.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_H
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_H
     */
    OPENING_NE,
    /**
     * @brief Moves to the north position.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_N
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_H
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_N
     */
    OPENING_NN,
    /**
     * @brief Moves to the north-west position
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_H
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_H
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_L
     */
    OPENING_NW,
    /**
     * @brief Moves to the south-east position while maintaining neutral rotation.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_N
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_H
     */
    OPENING_SEN,
    /**
     * @brief Moves to the south-east position.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_H
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_H
     */
    OPENING_SE,
    /**
     * @brief Moves to the south position.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_N
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_N
     */
    OPENING_SS,
    /**
     * @brief Moves to the south-west position.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_L
     */
    OPENING_SW,
    /**
     * @brief Moves to the south-west position while maintaining neutral rotation.
     *
     * Selected by the following code sequence:
     *
     * - @ref OPENING_STEP_0 : @ref OPENING_CODE_N
     *
     * - @ref OPENING_STEP_1 : @ref OPENING_CODE_L
     *
     * - @ref OPENING_STEP_2 : @ref OPENING_CODE_L
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
 * @brief Opening strategy selection step.
 *
 * The opening strategy is selected by sequentially measuring the @ref radio_t
 * channel values.
 */
typedef enum opening_step {
    OPENING_STEP_0 = 0,     /**< Opening strategy selection step 0. */
    OPENING_STEP_1,         /**< Opening strategy selection step 1. */
    OPENING_STEP_2,         /**< Opening strategy selection step 2. */
    NUMBER_OF_OPENING_STEPS /**< Number of opening steps. */
} opening_step_t;
 */
typedef struct opening_config {
    const char *name;           /**< Human-readable null-terminated opening strategy name. */
    const opening_code_t code[NUMBER_OF_OPENING_STEPS]; /**< Opening strategy code. */
} opening_config_t;

/**
 * @brief Opening strategy handler.
 */
typedef struct opening_handler {
    const char *name;                                   /**< Human-readable null-terminated opening handler name. */
    opening_code_t code[NUMBER_OF_OPENING_STEPS];       /**< Code sequence of the currently selected opening strategy. */
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
 * @brief Quantity of iterations required to select the opening strategy.
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
 * @brief Get current opening FSM state.
 *
 * @return Current opening FSM state.
 */
opening_state_t opening_get_status(void);

/**
 * @brief Run handler for @ref STATE_OPENING.
 */
void opening_run(void);
