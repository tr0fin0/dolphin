/**
 * @file pwm.h
 * @brief PWM utilities.
 *
 * @author Guilherme Nunes Trofino
 */

#pragma once

#include <stdint.h>

/**
 * @brief PWM pulse width in percentage with 0% begin `PWM_NEUTRAL_US`.
 */
typedef int8_t percentage_t;

/**
 * @brief PWM pulse width in microseconds.
 */
typedef int16_t pwm_pulse_t;

/**
 * @brief Normalized PWM pulse width in microseconds between `PWM_MINIMAL` and
 * `PWM_MAXIMAL` microseconds.
 */
typedef uint16_t pwm_pulse_norm_t;

#ifndef IRAM_ATTR
/**
 * @brief IRAM_ATTR is a macro used in ESP32 programming to place functions, particularly interrupt service routines (ISRs), in Instruction RAM (IRAM).
 */
#define IRAM_ATTR
#endif

/**
 * @brief Deadband around neutral position in microseconds.
 *
 * Pulses within `PWM_DEADBAND_US` distance of `PWM_NEUTRAL_US` are forced to
 * `PWM_NEUTRAL_US`.
 *
 * @note
 * - button debounce requires at least 50.
 */
#define PWM_DEADBAND_US       50U

#define PWM_DUTY_INITIAL       0.0F

/**
 * @brief Nominal servo frequency in Hz.
 */
#define PWM_FREQUENCY_HZ      50U

/**
 * @brief Maximum accepted pulse width in microseconds.
 */
#define PWM_MAXIMUM_US      2000U

/**
 * @brief Neutral pulse width in microseconds.
 */
#define PWM_NEUTRAL_US      1500U

/**
 * @brief Minimum accepted pulse width in microseconds.
 */
#define PWM_MINIMUM_US      1000U

/**
 * @brief 50% of the negative part of the pulse in microseconds.
 */
#define PWM_PERCENTAGE_M50 (PWM_NEUTRAL_US + PWM_MINIMUM_US) / 2

/**
 * @brief 50% of the positive part of the pulse in microseconds.
 */
#define PWM_PERCENTAGE_P50 (PWM_NEUTRAL_US + PWM_MAXIMUM_US) / 2

/**
 * @brief Normalize raw RC pulse width in microseconds.
 *
 * The function performs:
 *
 *  1. Saturation to [`PWM_MINIMUM_US`, `PWM_MAXIMUM_US`]
 *
 *  2. Neutral `PWM_DEADBAND_US` filtering around `PWM_NEUTRAL_US`
 *
 * This function is ISR-safe and placed in IRAM to allow execution from
 * interrupt context.
 *
 * @param pulse_us Raw measured pulse width in microseconds.
 *
 * @return Normalized pulse width in microseconds.
 */
pwm_pulse_norm_t IRAM_ATTR pwm_pulse_us_normalize(pwm_pulse_t pulse_us);

/**
 * @brief Converts PWM pulse percentage to a normalized pulse width in
 * microseconds.
 * 
 * The following equation converts PWM pulse percentage into pulse width:
 * 
 *  1. `PWM_PULSE_US = PWM_NEUTRAL_US + 5 * percentage`
 *
 * For example:
 *
 * - `PWM_MINIMUM_US` is -100%
 *
 * - `PWM_DEADBAND_US` is 0%
 *
 * - `PWM_MAXIMUM_US` is +100%.
 */
pwm_pulse_norm_t pwm_pulse_percentage(percentage_t percentage);
