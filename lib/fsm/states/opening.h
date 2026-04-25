/**
 * @file opening.h
 * @brief `STATE_OPENING` definition of the FSM callback functions `on_entry()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

/**
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
 */
void opening_entry(void);

/**
 * @brief Opening state run handler.
 *
 * Reads throttle value in 3 sequencial steps to determine the opening move
 * requested among the defined below:
 */
void opening_run(void);
