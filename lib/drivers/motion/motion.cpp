#include <Arduino.h>
#include "esc/esc.h"
#include "motion.h"
#include "motor/motor.h"
#include "pwm/pwm.h"

int8_t offset = 50;


void motion_rotation(angle_t angle) {
    if (angle == +45) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US - offset, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(24);
    } else if (angle == -45) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US + offset, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_R);

        delay(30);
    } else if (angle == -90) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US + offset, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_R);

        delay(36);
    } else if (angle == +90) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US - offset, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(36);
    } else if (angle == +180) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US - offset, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(72);
    }

    motors_set_pwm_neutral();
    delay(50);
}

void motion_translation(distance_t distance) {
    if (distance == -50) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US+75, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(135);
    } else if (distance == +50) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US+25, MOTOR_R);

        delay(115);
    }

    motors_set_pwm_neutral();
    delay(50);
}
