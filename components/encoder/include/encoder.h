/**
 * @file encoder.h
 * @brief Magnetic rotary encoder via PWM driver.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-07-27
 */

#pragma once

#include <stdint.h>
#include "pinout.h"
#include "pwm.h"

/**
 * @brief Encoder angle in radians between 0 and 2PI.
 */
typedef float encoder_ang_rad_t;

/**
 * @brief Encoder configuration.
 */
typedef struct encoder_config {
    const char *name;       /**< Human-readable null-terminated encoder name.*/
    pin_t pin;              /**< Encoder pin connection.*/
    pwm_t pwm;              /**< Encoder latest normalized PWM pulse width.*/
    int64_t rise_time_us;   /**< Encoder last rising time in microseconds.*/
} encoder_config_t;

/**
 * @brief Encoders positions.
 */
typedef enum encoder {
    ENCODER_L = 0,      /**< Encoder on the left wheel. */
    ENCODER_R,          /**< Encoder on the right wheel. */
    NUMBER_OF_ENCODERS  /**< Number of encoder positions.*/
} encoder_t;

/**
 * @def ENCODER_PWM_MAX_US
 * @brief Encoder maximum normalized PWM pulse width in microseconds.
 *
 * **Default Value:** 1000.0
 */
#define ENCODER_PWM_MAX_US 1000.0f

/**
 * @def ENCODER_PWM_MIN_US
 * @brief Encoder minimum normalized PWM pulse width in microseconds.
 *
 * **Default Value:** 0.0
 */
#define ENCODER_PWM_MIN_US    0.0f

/**
 * @brief Initialization of all Encoders interrupts.
 */
void encoder_init(void);

/**
 * @brief Returns the Encoder name as a null-terminated string.
 *
 * @param[in] encoder Encoder position.
 *
 * @return Human-readable null-terminated string representing the encoder name.
 */
const char *encoder_get_name(encoder_t encoder);

/**
 * @brief Return latest measured angle in radians from encoder.
 *
 * @param[in] encoder Encoder position.
 *
 * @return Angle in radians between 0 and 2PI.
 *
 * @note Interruptions briefly disabled while copying values.
 */
encoder_ang_rad_t encoder_read_angle(encoder_t encoder);
