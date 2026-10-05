/**
 * @file controller.h
 * @brief Motion controller engine.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-25
 */

#pragma once

#include "pwm.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Type of motion performed by the controller.
 */
typedef enum controller_motion {
    CONTROLLER_MOTION_ROTATION = 0,
    CONTROLLER_MOTION_STOP,
    CONTROLLER_MOTION_TRANSLATION,
    NUMBER_OF_CONTROLLER_MOTIONS    /**< Number of controller motions. */
} controller_motion_t;

/**
 * @brief Controller operation state.
 */
typedef enum controller_state {
    CONTROLLER_STATE_ACTIVE = 0,    /**< Action is happening. */
    CONTROLLER_STATE_IDLE,          /**< No action is happening. */
    NUMBER_OF_CONTROLLER_STATES     /**< Number of controller states. */
} controller_state_t;

/**
 * @brief Motion command.
 */
typedef struct controller_command {
    controller_motion_t motion; /**< Type of motion to perform. */
    pwm_percentage_t power;     /**< Signed PWM command. */
    int64_t duration_us;        /**< Command duration in microseconds. */
} controller_command_t;

/**
 * @brief Sequence of controller commands.
 */
typedef struct controller_sequence {
    const controller_command_t *commands;   /**< Array of commands. */
    uint8_t length;                         /**< Number of commands. */
    bool repeat;                            /**< Repeats sequence after the last command. */
} controller_sequence_t;

/**
 * @brief Controller engine.
 */
typedef struct controller {
    const char *name;                                       /**< Human-readable, null-terminated name of the controller. */
    controller_state_t state;                               /**< Current controller state. */
    const char *states_names[NUMBER_OF_CONTROLLER_STATES];  /**< Array of human-readable, null-terminated names of the controller states. */
    const controller_sequence_t *sequence;                  /**< Current sequence of controller commands. */
    uint8_t command_current;                                /**< Current controller command. */
    int64_t command_start_us;                               /**< Current controller command start time in microseconds. */
} controller_t;

/**
 * @brief Returns current controller state.
 *
 * @return Current controller state.
 */
controller_state_t controller_get_state(void);

/**
 * @brief Controller initialization.
 *
 * Calls ``controller_stop()``.
 */
void controller_init(void);

/**
 * @brief Controller start a command sequence.
 *
 * Initialize a sequence of motion commands.
 *
 * @param[in] sequence Pointer to sequence of controller commands.
 *
 * @return True if sequence of commands **is not** ``null`` and the current
 * controller state is ``CONTROLLER_STATE_IDLE``. Otherwise, returns false and
 * no motion is performed.
 */
bool controller_start(const controller_sequence_t *sequence);

/**
 * @brief Verifies current controller state and update if required.
 */
void controller_step(void);

/**
 * @brief Controller stop a command sequence.
 *
 * Calls ``esc_set_pwm_mix_neutral()``, reinitialize the current command
 * sequence, and sets the current controller state to ``CONTROLLER_STATE_IDLE``.
 */
void controller_stop(void);
