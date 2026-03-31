#ifndef __ESC_H__
#define __ESC_H__

#include <stdio.h>
#include "motor/motor.h"
#include "pwm/pwm.h"



#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Initialize MCPWM driver for ESC.
 *
 * Configures MCPWM Unit 0 / Timer 0 for standard RC servo timing of 50 Hz, 20
 * ms period.
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
 * 
 */
void esc_validate(void);



#ifdef __cplusplus
}
#endif

#endif /* __ESC_H__ */
