#ifndef __ESC_H__
#define __ESC_H__

#include <stdio.h>
#include "pwm/pwm.h"



#ifdef __cplusplus
extern "C" {
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
 * @brief Initialize MCPWM driver for ESC.
 *
 * Configures MCPWM Unit 0 / Timer 0 for standard RC servo timing of 50 Hz, 20
 * ms period.
 *
 * @note
 * stabilization delay of 500 ms applied to allow ESC arming.
 */
void esc_init(void);

/**
 * @brief Set PWM pulse width for a single motor.
 *
 * Writes a pulse width in microseconds directly to the MCPWM hardware using
 * `MOTOR_OPERATOR(motor)`.
 *
 * @param pulse_us Desired pulse width in microseconds.
 * @param motor    Motor identifier (`motor_t`).
 *
 * @note
 * - designed for 50 Hz RC ESC signals between 1000–2000 microseconds.
 *
 * - `pwm_pulse_us_normalize()` to ensure safe ESC limits.
 */
void esc_set_pwm(pwm_pulse_t pulse_us, motor_t motor);

/**
 * @brief Apply differential drive mixing and update both motors.
 *
 * Implements skid-steering (tank drive) mixing:
 *
 * - `left ` = throttle + steering
 *
 * - `right` = throttle - steering
 *
 * @param pulses_us Input array:
 *
 *        - pulses_us[0]: Throttle in microseconds
 *
 *        - pulses_us[1]: Steering in microseconds
 */
void esc_set_pwms(pwm_pulse_norm_t pulses_us[NUMBER_OF_MOTORS]);


void esc_validate(void);



#ifdef __cplusplus
}
#endif

#endif /* __ESC_H__ */
