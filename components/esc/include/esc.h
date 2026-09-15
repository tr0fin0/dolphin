/**
 * @file esc.h
 * @brief Electronic Speed Controller for Brushless Direct Current motors.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-04-28
 */

#pragma once

#include "driver/mcpwm_prelude.h"
#include "pwm.h"
#include "pinout.h"

/**
 * @brief ESC configuration.
 */
typedef struct esc_config {
    const char *name;               /**< Human-readable null-terminated ESC name. */
    pin_t pin;                      /**< ESC pin connection. */
    mcpwm_cmpr_handle_t comparator; /**< ESC MCPWM comparator handle. */
} esc_config_t;

/**
 * @brief ESC positions.
 */
typedef enum esc {
    ESC_L = 0,      /**< ESC connected to the left motor. */
    ESC_R,          /**< ESC connected to the right motor. */
    NUMBER_OF_ESCS  /**< Number of ESC positions on the project. */
} esc_t;

/**
 * @def ESC_GROUP_ID
 * @brief ESCs MCPWM Timer Group ID.
 *
 * All ESCs are configured to share the same MCPWM Timer.
 *
 * **Default Value:** 0
 */
#define ESC_GROUP_ID            0

/**
 * @def ESC_RESOLUTION_HZ
 * @brief ESCs MCPWM Timer Resolution.
 *
 * All ESCs are configured to operate with the same MCPWM Timer Resolution.
 *
 * **Default Value:** 1 MHz
 */
#define ESC_RESOLUTION_HZ 1000000

/**
 * @def ESC_PERIOD_US
 * @brief ESCs MCPWM Timer Period.
 *
 * All ESCs are configured to operate with the same MCPWM Timer Period.
 *
 * **Default Value:** 20 ms
 */
#define ESC_PERIOD_US       20000

/**
 * @brief Returns the ESC name.
 *
 * @param[in] esc ESC position.
 * @return Human-readable null-terminated string representing the name.
 */
const char *esc_get_name(esc_t esc);

/**
 * @brief Initialize all ESCs to the same MCPWM driver.
 *
 * Uses MCPWM Unit 0 on Timer 0 for standard RC timing of 50 Hz, 20 ms period.
 */
void esc_init(void);

/**
 * @brief Set PWM pulse width for a ESC.
 *
 * PWM pulse normalized via @ref pwm_normalize to ensure safe ESC limits.
 *
 * @param[in] esc ESC position.
 * @param[in] pwm PWM pulse width in microseconds.
 */
void esc_set_pwm(esc_t esc, pwm_t pwm);

/**
 * @brief Set PWM pulse width for both ESCs via differential drive mixing.
 *
 * Implements tank drive mixing as follows:
 *
 * ```text
 * l = steering + throttle - PWM_NEUTRAL_US
 * r = throttle - steering + PWM_NEUTRAL_US
 * ```
 *
 * @param[in] pwms Array of PWM pulses widths for steering and throttle:
 *                 - `pwms[0]`: steering in microseconds
 *                 - `pwms[1]`: throttle in microseconds
 */
void esc_set_pwm_mix(pwm_norm_t *pwms);

/**
 * @brief PWM pulse width to @ref PWM_NEUTRAL_US for both ESCs.
 */
void esc_set_pwm_mix_neutral();

/**
 * @brief Set PWM pulse width to @ref PWM_NEUTRAL_US for a ESC.
 *
 * @param[in] esc ESC position.
 */
void esc_set_pwm_neutral(esc_t esc);
