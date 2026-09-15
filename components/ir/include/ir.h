/**
 * @file ir.h
 * @brief Infrared receiver device.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-06
 */

#pragma once

#include "pinout.h"
#include <stdint.h>

/**
 * @brief Infrared states.
 */
typedef enum ir_state {
    IR_STATE_STANDBY = 0,   /**< Infrared receiver received no command. */
    IR_STATE_START,         /**< Infrared receiver received the start command. */
    IR_STATE_STOP,          /**< Infrared receiver received the stop command . */
    NUMBER_OF_IR_STATES     /**< Number of infrared states. */
} ir_state_t;

/**
 * @brief Infrared receiver device.
 */
typedef struct ir {
    const char *name;                               /**< Human-readable null-terminated infrared name. */
    const char *state_names[NUMBER_OF_IR_STATES];   /**< Human-readable null-terminated infrared state names. */
    volatile ir_state_t state;                      /**< Infrared state. */
    pin_t pin;                                      /**< Infrared pin connection. */
} ir_t;

/**
 * @brief Returns the infrared name.
 *
 * @return Human-readable null-terminated string representing the name.
 */
const char *ir_get_name(void);

/**
 * @brief Returns the infrared current state.
 *
 * @return Current infrared state.
 */
ir_state_t ir_get_state(void);

/**
 * @brief Returns the infrared current state name.
 *
 * @return Human-readable null-terminated string representing the state name.
 */
const char *ir_get_state_name(void);

/**
 * @brief Initializes the infrared interruption.
 */
void ir_init(void);
