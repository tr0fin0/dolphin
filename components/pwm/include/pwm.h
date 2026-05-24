/**
 * @file pwm.h
 * @brief PWM utilities.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-03
 */

#pragma once

#include <stdint.h>
#include "esp_attr.h"

/**
 * @brief PWM pulse width in microseconds.
 */
typedef int16_t pwm_t;

/**
 * @brief PWM pulse width in microseconds normalized between `PWM_MINIMUM_US`
 * and `PWM_MAXIMUM_US`.
 */
typedef uint16_t pwm_norm_t;

/**
 * @brief PWM pulse width in percentage with:
 *
 * - `PWM_MINIMUM_US` being -100%
 *
 * - `PWM_NEUTRAL_US` being 0%
 *
 * - `PWM_MAXIMUM_US` being +100%.
 */
typedef int8_t pwm_percentage_t;

/**
 * @brief Deadband distance around maximum, minimum and neutral positions in
 * microseconds.
 *
 * Pulses within `PWM_DEADBAND_US` distance of `PWM_MINIMUM_US`, `PWM_NEUTRAL_US`,
 * or `PWM_MAXIMUM_US` are truncated to their respective values.
 *
 * @note
 * - button debounce requires at least 50 microseconds deadband.
 */
#define PWM_DEADBAND_US       50U

/**
 * @brief PWM duty cycle initialization value.
 */
#define PWM_DUTY_INITIAL       0.0F

/**
 * @brief Nominal servo frequency in Hz.
 */
#define PWM_FREQUENCY_HZ      50U

/**
 * @brief Maximum accepted pulse width in percentage.
 */
#define PWM_MAXIMUM_PERCENTAGE +100

/**
 * @brief Maximum accepted pulse width in microseconds.
 */
#define PWM_MAXIMUM_US      2000U

/**
 * @brief Minimum accepted pulse width in percentage.
 */
#define PWM_MINIMUM_PERCENTAGE -100

/**
 * @brief Minimum accepted pulse width in microseconds.
 */
#define PWM_MINIMUM_US      1000U

/**
 * @brief Neutral pulse width in microseconds.
 */
#define PWM_NEUTRAL_US      1500U

/**
 * @brief Normalize PWM pulse width in microseconds.
 *
 * The function performs:
 *
 *  1. PWM saturation to [`PWM_MINIMUM_US`, `PWM_MAXIMUM_US`]
 *
 *  2. `PWM_DEADBAND_US` filtering around `PWM_MAXIMUM_US`
 *
 *  3. `PWM_DEADBAND_US` filtering around `PWM_NEUTRAL_US`
 *
 *  4. `PWM_DEADBAND_US` filtering around `PWM_MINIMUM_US`
 *
 * Function placed in IRAM to allow execution from interrupt context.
 *
 * @param pwm PWM pulse width in microseconds.
 *
 * @return PWM pulse width in microseconds normalized.
 */
pwm_norm_t IRAM_ATTR pwm_normalize(pwm_t pwm);

/**
 * @brief PWM pulse percentage convertion to normalized PWM pulse width in
 * microseconds.
 * 
 * The following equation converts PWM pulse percentage into PWM pulse width:
 * 
 *  1. `PWM_PULSE_US = PWM_NEUTRAL_US + 5 * percentage`
 *
 * For example:
 *
 * - `PWM_MINIMUM_US` is -100%
 *
 * - `PWM_NEUTRAL_US` is 0%
 *
 * - `PWM_MAXIMUM_US` is +100%.
 *
 * @param pwm_percentage PWM pulse width in percentage.
 *
 * @return PWM pulse width in microseconds normalized.
 */
pwm_norm_t pwm_percentage(pwm_percentage_t pwm_percentage);
