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
 * @brief Encoder configuration
 */
typedef struct encoder_config {
    const char *name;
    pin_t pin;
    pwm_t pwm;
    int64_t rise_time_us;
} encoder_config_t;

/**
 * @brief Encoders positions.
 *
 * Available values are:
 *
 * - `ENCODER_L`
 *
 * - `ENCODER_R`
 */
typedef enum encoder {
    ENCODER_L = 0,  /** Encoder on the left wheel */
    ENCODER_R,      /** Encoder on the right wheel */
    NUMBER_OF_ENCODERS
} encoder_t;

#define ENCODER_PWM_MAX_US 1000.0f
#define ENCODER_PWM_MIN_US    0.0f

/**
 * @brief Initialization of all Encoders interrupts.
 */
void encoder_init(void);

/**
 * @brief Returns the Encoder name as a null-terminated string.
 */
const char *encoder_get_name(encoder_t encoder);

/**
 * @brief Return latest measured angle in radians from encoder.
 *
 * @param encoder Encoder position.
 *
 * @return Angle in radians between 0 and 2PI.
 *
 * @note
 * Interruptions briefly disabled while copying values.
 */
encoder_ang_rad_t encoder_read_angle(encoder_t encoder);
