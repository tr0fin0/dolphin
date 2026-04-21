#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include "pwm.h"

#ifdef __cplusplus
extern "C"
{
#endif



/**
 * @brief Logical motor identifiers.
 *
 * Defines the ordered mapping of drive motors in the system.
 *
 * - `MOTOR_L`: left motor.
 *
 * - `MOTOR_R`: right motor.
 */
typedef enum {
    MOTOR_L,
    MOTOR_R,
    NUMBER_OF_MOTORS
} motor_t;

/**
 * @brief Resolve MCPWM operator corresponding to a motor.
 *
 * Maps a `motor_t` to its associated MCPWM operator:
 *
 *  - `MOTOR_L`:    `MCPWM_OPR_A`
 *
 *  - `MOTOR_R`:    `MCPWM_OPR_B`
 *
 * @param motor Motor identifier (`motor_t`).
 *
 * @return MCPWM operator (`mcpwm_operator_t`).
 */
#define MOTOR_OPERATOR(motor)   ((motor) == MOTOR_L ? MCPWM_OPR_A : MCPWM_OPR_B)

/**
 * @brief Resolve GPIO pin corresponding to a motor.
 *
 * Maps a `motor_t` to its configured pin:
 *
 *  - `MOTOR_L`:    `PIN_ESC_L`
 *
 *  - `MOTOR_R`:    `PIN_ESC_R`
 *
 * @param motor Motor identifier (`motor_t`).
 *
 * @return GPIO number used for PWM output.
 */
#define MOTOR_PIN(motor)        ((motor) == MOTOR_L ? PIN_ESC_L   : PIN_ESC_R  )

/**
 * @brief Resolve MCPWM signal corresponding to a motor.
 *
 * Maps a `motor_t` to the corresponding MCPWM output signal:
 *
 *  - `MOTOR_L`:    `MCPWM0A`
 *  - `MOTOR_R`:    `MCPWM0B`
 *
 * @param motor Motor identifier (`motor_t`).
 *
 * @return MCPWM I/O signal identifier.
 */
#define MOTOR_PWM(motor)        ((motor) == MOTOR_L ? MCPWM0A     : MCPWM0B    )



/**
 * @brief Stop motor by commanding neutral PWM.
 *
 * Set ESC input to `PWM_NEUTRAL_US`, ensuring zero PWM condition.
 */
void motor_set_pwm_neutral(motor_t motor);

/**
 * @brief Apply differential drive mixing and update both motors.
 *
 * Implements skid-steering (tank drive) mixing:
 *
 * - `l = throttle + steering - PWM_NEUTRAL_US`
 *
 * - `r = throttle - steering + PWM_NEUTRAL_US`
 *
 * @param pulses_us array of radio pulses:
 *
 *        - pulses_us[0]: steering in microseconds
 *
 *        - pulses_us[1]: throttle in microseconds
 */
void motors_set_pwm(pwm_pulse_norm_t pulses_us[NUMBER_OF_MOTORS]);

/**
 * @brief Stop both motors by commanding neutral PWM.
 *
 * Set both ESCs inputs to `PWM_NEUTRAL_US`, ensuring zero PWM condition.
 */
void motors_set_pwm_neutral();



#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_H__ */
