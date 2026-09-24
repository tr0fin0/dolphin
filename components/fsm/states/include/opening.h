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
 * Uses @ref radio_t signals from 2 channels to determine the opening code.
 * Typically, the throttle channel value is measured when the button channel is
 * pressed.
 */
typedef enum opening_code {
    /**
     * @brief Represents the high @ref pwm_norm_t level.
     *
     * Selected when the radio receiver channel value is greater than:
     *
     * - ``( PWM_NEUTRAL_US + PWM_MAXIMUM_US ) / 2``.
     */
    OPENING_CODE_H = 0,
    /**
     * @brief Represents the low @ref pwm_norm_t level.
     *
     * Selected when the radio receiver channel value is less than:
     *
     * - ``( PWM_NEUTRAL_US + PWM_MINIMUM_US ) / 2``.
     */
    OPENING_CODE_L,
    /**
     * @brief Represents the neutral @ref pwm_norm_t level.
     *
     * Selected when the radio receiver channel value is between inclusive:
     *
     * - ``( PWM_NEUTRAL_US + PWM_MINIMUM_US ) / 2``
     *
     * - ``( PWM_NEUTRAL_US + PWM_MAXIMUM_US ) / 2``
     */
    OPENING_CODE_N,
    NUMBER_OF_OPENING_CODES /**< Number of openings codes. */
} opening_code_t;

/**
 * @brief Opening Finite State Machine states.
 *
 * The opening strategy is executed regardless of the selected @ref config_control_mode_t.
 */
typedef enum opening_state {
    OPENING_STATE_EXECUTION = 0,    /**< The selected opening strategy is being executed. */
    OPENING_STATE_FINISHED,         /**< The selected opening strategy has finished executing. */
    OPENING_STATE_RELEASE,          /**< The opening strategy has finished executing and the FSM is waiting for the release command. */
    OPENING_STATE_SELECTION,        /**< The opening strategy is being selected. */
    NUMBER_OF_OPENING_STATES,       /**< Number of opening FSM states. */
} opening_state_t;

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

/**
 * @brief Configuration for an opening strategy.
 */
typedef struct opening_config {
    const char *name;                                   /**< Human-readable, null-terminated name of the opening strategy. */
    const opening_code_t code[NUMBER_OF_OPENING_STEPS]; /**< Opening strategy code. */
} opening_config_t;

/**
 * @brief Opening strategy handler.
 */
typedef struct opening_handler {
    const char *name;                                   /**< Human-readable null-terminated opening handler name. */
    opening_code_t code[NUMBER_OF_OPENING_STEPS];       /**< Code sequence of the currently selected opening strategy. */
    const char *codes_names[NUMBER_OF_OPENING_CODES];   /**< Human-readable, null-terminated names of the opening codes. */
    opening_step_t step;                                /**< Opening handler strategy selection step. */
    opening_state_t state;                              /**< Opening handler state. */
    const char *states_names[NUMBER_OF_OPENING_STATES]; /**< Human-readable null-terminated opening handler states names. */
    pwm_norm_t last_button;                             /**< Opening handler last button measure. */
} opening_handler_t;

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
 * @brief Returns the opening code name.
 *
 * @return Human-readable null-terminated string representing the code name.
 */
const char *opening_get_code_name(opening_code_t code);

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
