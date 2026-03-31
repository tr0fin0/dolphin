#include <Arduino.h>
#include "esc/esc.h"
#include "motion.h"
#include "motor/motor.h"
#include "pwm/pwm.h"



void motion_rotate(angle_t angle) {
    if (angle == -45) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_R);

        delay(40);
    } else if (angle == +45) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(60);
    } else if (angle == -90) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_R);

        delay(60);
    } else if (angle == +90) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(100);
    } else if (angle == -180) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US, MOTOR_R);

        delay(125);
    } else if (angle == +180) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(150);
    }
}

void motion_translation(distance_t distance) {
    if (distance == -50) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US + 120, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_R);

        delay(500);
    } else if (distance == +50) {
        esc_set_pwm((pwm_pulse_norm_t) PWM_MAXIMUM_US, MOTOR_L);
        esc_set_pwm((pwm_pulse_norm_t) PWM_MINIMUM_US + 75, MOTOR_R);

        delay(200);
    }
}
