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
 * @brief PWM pulse width in microseconds normalized between @ref PWM_MINIMUM_US
 * and @ref PWM_MAXIMUM_US.
 */
typedef uint16_t pwm_norm_t;

/**
 * @brief PWM pulse width in percentage.
 *
 * Where:
 *
 * - @ref PWM_MINIMUM_US being `-100%`
 *
 * - @ref PWM_NEUTRAL_US being `0%`
 *
 * - @ref PWM_MAXIMUM_US being `+100%`
 */
typedef int8_t pwm_percentage_t;

/**
 * @def PWM_DEADBAND_US
 * @brief Deadband distance around maximum, minimum and neutral positions in
 * microseconds.
 *
 * Pulses within `PWM_DEADBAND_US` distance of @ref PWM_MINIMUM_US,
 * @ref PWM_NEUTRAL_US, or @ref PWM_MAXIMUM_US are truncated to their respective
 * values.
 *
 * @note Button debounce requires at least 50 microseconds of deadband.
 *
 * **Default Value:** 50
 */
#define PWM_DEADBAND_US       50U

/**
 * @def PWM_DUTY_INITIAL
 * @brief PWM duty cycle initialization value.
 *
 * **Default Value:** 0.0
 */
#define PWM_DUTY_INITIAL       0.0F

/**
 * @def PWM_FREQUENCY_HZ
 * @brief PWM Nominal frequency in Hz.
 *
 * **Default Value:** 50 Hz
 */
#define PWM_FREQUENCY_HZ      50U

/**
 * @def PWM_MAXIMUM_US
 * @brief PWM Maximum accepted pulse width in microseconds.
 *
 * **Default Value:** 2000
 */
#define PWM_MAXIMUM_US      2000U

/**
 * @def PWM_MINIMUM_US
 * @brief PWM Minimum accepted pulse width in microseconds.
 *
 * **Default Value:** 1000
 */
#define PWM_MINIMUM_US      1000U

/**
 * @def PWM_NEUTRAL_US
 * @brief PWM Neutral pulse width in microseconds.
 *
 * **Default Value:** 1500
 */
#define PWM_NEUTRAL_US      1500U

/**
 * @brief Normalize PWM pulse width in microseconds.
 *
 * The function performs:
 *
 *  1. PWM saturation to [ @ref PWM_MINIMUM_US, @ref PWM_MAXIMUM_US ]
 *
 *  2. @ref PWM_DEADBAND_US filtering around @ref PWM_MAXIMUM_US.
 *
 *  3. @ref PWM_DEADBAND_US filtering around @ref PWM_NEUTRAL_US.
 *
 *  4. @ref PWM_DEADBAND_US filtering around @ref PWM_MINIMUM_US.
 *
 * Function placed in IRAM to allow execution from interrupt context.
 *
 * @param[in] pwm PWM pulse width in microseconds.
 *
 * @return Normalized PWM pulse width in microseconds.
 */
pwm_norm_t IRAM_ATTR pwm_normalize(pwm_t pwm);

/**
 * @brief Convert PWM pulse percentage to a normalized PWM pulse width in
 * microseconds.
 *
 * The following equation converts the percentage into a pulse width:
 *
 * ```text
 * PWM_PULSE_US = PWM_NEUTRAL_US + (5 * percentage)
 * ```
 *
 * For example:
 *
 * - @ref PWM_MINIMUM_US is -100%
 *
 * - @ref PWM_NEUTRAL_US is 0%
 *
 * - @ref PWM_MAXIMUM_US is +100%.
 *
 * @param[in] pwm_percentage PWM pulse width in percentage.
 *
 * @return Normalized PWM pulse width in microseconds.
 */
pwm_norm_t pwm_percentage(pwm_percentage_t pwm_percentage);
