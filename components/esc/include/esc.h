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

#define ESC_GROUP_ID            0 /** MCPWM Timer Group ID */
#define ESC_RESOLUTION_HZ 1000000 /** MCPWM Timer Resolution of 1 MHz */
#define ESC_PERIOD_US       20000 /** MCPWM Timer Period of 20ms */

/**
 * @brief ESC configuration.
 */
typedef struct {
    const char *name;
    pin_t pin;
    mcpwm_cmpr_handle_t comparator;
} esc_config_t;

/**
 * @brief ESCs on the project.
 *
 * Available values are:
 *
 * - `ESC_L`
 *
 * - `ESC_R`
 */
typedef enum {
    ESC_L = 0,      /** ESC connected to the left motor*/
    ESC_R,          /** ESC connected to the right motor*/
    NUMBER_OF_ESCS
} esc_t;

/**
 * @brief Initialize all ESCs to the same MCPWM driver.
 *
 * Uses MCPWM Unit 0 on Timer 0 for standard RC timing of 50 Hz, 20 ms period.
 */
void esc_init(void);

/**
 * @brief Set PWM pulse width for a ESC.
 *
 * PWM pulse normalized via `pwm_normalize()` to ensure safe ESC limits.
 *
 * @param esc ESC on the project.
 * @param pwm PWM pulse width in microseconds.
 */
void esc_set_pwm(esc_t esc, pwm_t pwm);

/**
 * @brief Set PWM pulse width for both ESCs via differential drive mixing.
 *
 * Implements tank drive mixing:
 *
 * - `l = steering + throttle - PWM_NEUTRAL_US`
 *
 * - `r = steering - steering + PWM_NEUTRAL_US`
 *
 * @param pwms array of PWM pulses widths:
 *
 * - pwms[0]: steering in microseconds
 *
 * - pwms[1]: throttle in microseconds
 */
void esc_set_pwm_mix(pwm_norm_t pwms[NUMBER_OF_ESCS]);

/**
 * @brief PWM pulse width to `PWM_NEUTRAL_US` for both ESCs.
 */
void esc_set_pwm_mix_neutral();

/**
 * @brief Set PWM pulse width to `PWM_NEUTRAL_US` for a ESC.
 *
 * @param esc ESC on the project.
 */
void esc_set_pwm_neutral(esc_t esc);
