#include "debug.h"
#include "esc.h"
#include "motor.h"
#include "pwm.h"



void motor_set_pwm_neutral(motor_t motor) {
    esc_set_pwm((pwm_pulse_t) PWM_NEUTRAL_US, motor);
}

void motors_set_pwm(pwm_pulse_norm_t pulses_us[NUMBER_OF_MOTORS]) {
    pwm_pulse_norm_t steering_us = pulses_us[0];
    DEBUG_MSG(DEBUG_LEVEL_TRACE, "measured steering: %d us", steering_us);

    pwm_pulse_norm_t throttle_us = pulses_us[1];
    DEBUG_MSG(DEBUG_LEVEL_TRACE, "measured throttle: %d us", throttle_us);

    esc_set_pwm((pwm_pulse_t) steering_us + throttle_us - PWM_NEUTRAL_US, MOTOR_L);
    esc_set_pwm((pwm_pulse_t) steering_us - throttle_us + PWM_NEUTRAL_US, MOTOR_R);
};

void motors_set_pwm_neutral() {
    motor_set_pwm_neutral(MOTOR_L);
    motor_set_pwm_neutral(MOTOR_R);
};
