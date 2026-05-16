/**
 * @file opening.h
 * @brief `STATE_OPENING` definition of the FSM callback functions `on_entry()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

/**
 * @brief Defines quantity of iterations required to select the opening strategy
 * in the Radio Controlled mode.
 */
#define OPENING_ITERATIONS 3

/**
 * @brief Radio Controlled possible opening moves.
 *
 * Each digit from left to right represents the measure throttle value at the
 * respective iteration.
 *
 * @note Omitted digits should be considered as `0`.
 *
 * @example An opening strategy of value `210` is obtained by having a throttle
 * channel value of `2` in the first measure, `1` in the second measure and `0`
 * in the third measure.
 */
typedef enum {
    OPENING_STATIC =   0, /** Remains in the initial position and rotation. */
    OPENING_DRAW   =   1, /** Remains in the initial position and rotate 180 degrees. */
    OPENING_NE     = 221, /** From the initial position goes to north-east. */
    OPENING_NN     = 20,  /** From the initial position goes to north. */
    OPENING_NW     = 122, /** From the initial position goes to north-west. */
    OPENING_SEN    = 210, /** From the initial position goes to south-east with neutral rotation. */
    OPENING_SE     = 212, /** From the initial position goes to south-east. */
    OPENING_SS     =  10, /** From the initial position goes to south. */
    OPENING_SW     = 111, /** From the initial position goes to south-west. */
    OPENING_SWN    = 110, /** From the initial position goes to south-west with neutral rotation. */
} opening_t;

/**
 * @brief Entry handler for `STATE_OPENING`.
 *
 * Set `LED_STATE` to `COLOR_PURPLE` and captures current Radio Controller
 * button value.
 */
void opening_entry(void);

/**
 * @brief Run handler for `STATE_OPENING`.
 *
 * While Radio Controller is connected, 3 sequencial reads of the throttle
 * channel are used to determine the opening move.
 *
 * @note
 * - `2` if throttle is more than `PWM_PERCENTAGE_P50`.
 *
 * - `0` if throttle is between `PWM_PERCENTAGE_M50` and `PWM_PERCENTAGE_P50`.
 *
 * - `1` if throttle is less than `PWM_PERCENTAGE_M50`.
 */
void opening_run(void);
