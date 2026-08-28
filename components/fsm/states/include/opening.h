/**
 * @file opening.h
 * @brief STATE_OPENING definition of the FSM callback functions `on_entry()`
 * and `on_run()`.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#include "odometry.h"

/**
 * @brief Opening 3 digit code.
 *
 * @note Possible individual digit values are:
 *
 * - `3` if radio receiver channel is `> (PWM_NEUTRAL_US + PWM_MAXIMUM_US) / 2`.
 *
 * - `1` if radio receiver channel is `< (PWM_NEUTRAL_US + PWM_MINIMUM_US) / 2`.
 *
 * - `2` if radio receiver channel is not in the previous intervals.
 */
typedef uint16_t opening_code_t;

/**
 * @brief Opening interation step.
 */
typedef uint8_t opening_step_t;

/**
 * @brief
 */
typedef struct opening_config {
    const char *name;
    const opening_code_t code;
    odometry_waypoint_t waypoints;
} opening_config_t;

/**
 * @brief Radio Controlled possible opening moves.
 *
 * Each digit from left to right represents the measure throttle value at the
 * respective iteration.
 *
 * Available values are:
 *
 * - `OPENING_STATIC`
 *
 * - `OPENING_DRAW`
 *
 * - `OPENING_NE`
 *
 * - `OPENING_NN`
 *
 * - `OPENING_NW`
 *
 * - `OPENING_SEN`
 *
 * - `OPENING_SE`
 *
 * - `OPENING_SS`
 *
 * - `OPENING_SW`
 *
 * - `OPENING_SWN`
 *
 * An opening strategy of value 321` is obtained by having a throttle
 * channel value of `3` in the first measure, `2` in the second measure and `1`
 * in the third measure.
 *
 * @note Omitted digits should be considered as `0`.
 */
typedef enum opening {
    OPENING_STATIC = 0, /** 222: Remains in the initial position and rotation. */
    OPENING_DRAW,       /** 221: Remains in the initial position and rotate 180 degrees. */
    OPENING_NE,         /** 331: Goes to north-east. */
    OPENING_NN,         /** 232: Goes to north. */
    OPENING_NW,         /** 133: Goes to north-west. */
    OPENING_SEN,        /** 312: Goes to south-east with neutral rotation. */
    OPENING_SE,         /** 313: Goes to south-east. */
    OPENING_SS,         /** 212: Goes to south. */
    OPENING_SW,         /** 111: Goes to south-west. */
    OPENING_SWN,        /** 112: Goes to south-west with neutral rotation. */
    NUMBER_OF_OPENINGS
} opening_t;

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
 * Set @ref LED_STATE to @ref COLOR_PURPLE and captures current Radio Controller
 * button value.
 */
void opening_entry(void);

/**
 * @brief Run handler for @ref STATE_OPENING.
 *
 * While Radio Controller is connected, 3 sequential reads of the throttle
 * channel are used to determine the opening move.
 */
void opening_run(void);
