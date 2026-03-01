/**
 * @file pwm.h
 * @brief RC PWM pulse normalization utilities.
 *
 * Provides utilities for:
 * - Saturating pulse widths to valid servo range.
 * - Applying neutral deadband filtering.
 *
 * Designed for ISR-safe usage (IRAM compatible).
 */

#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>



#ifdef __cplusplus
extern "C" {
#endif



/**
 * @typedef pwm_pulse_t
 * @brief PWM pulse width in microseconds.
 */
typedef int16_t pwm_pulse_t;

/**
 * @typedef pwm_pulse_norm_t
 * @brief Normalized PWM pulse width in microseconds between `PWM_MINIMAL` and
 * `PWM_MAXIMAL` microseconds.
 */
typedef uint32_t pwm_pulse_norm_t;

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



#ifdef __cplusplus
}
#endif

#endif /* __PWM_H__ */
