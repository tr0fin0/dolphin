#include <Arduino.h>
#include <driver/mcpwm.h>
#include "esc.h"
#include "pinout.h"
#include "pwm/pwm.h"
#include "radio/radio.h"



void esc_init() {
    mcpwm_config_t pwm_config;
    pwm_config.frequency    = PWM_FREQUENCY_HZ;     // 50 Hz => 20 ms period
    pwm_config.cmpr_a       = PWM_DUTY_INITIAL;     // initial duty operator A
    pwm_config.cmpr_b       = PWM_DUTY_INITIAL;     // initial duty operator B
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode    = MCPWM_DUTY_MODE_0;

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);


    motor_t motors[NUMBER_OF_MOTORS] = {
        MOTOR_L,
        MOTOR_R
    };

    for (uint8_t i = 0; i < NUMBER_OF_MOTORS; i++) {
        pinMode(motors[i], OUTPUT);

        mcpwm_gpio_init(
            MCPWM_UNIT_0,
            MOTOR_PWM(motors[i]),
            MOTOR_PIN(motors[i])
        );

        (void) mcpwm_set_duty_in_us(
            MCPWM_UNIT_0,
            MCPWM_TIMER_0,
            MOTOR_OPERATOR(motors[i]),
            (pwm_pulse_norm_t) PWM_NEUTRAL_US
        );

        mcpwm_set_duty_type(
            MCPWM_UNIT_0,
            MCPWM_TIMER_0,
            MOTOR_OPERATOR(motors[i]),
            MCPWM_DUTY_MODE_0
        );
    }

    delay(500); // initialization stabilization
};


void esc_set_pwm(pwm_pulse_t pulse_us, motor_t motor) {
    pwm_pulse_norm_t pulse_norm_us = pwm_pulse_us_normalize(pulse_us);

    (void) mcpwm_set_duty_in_us(
        MCPWM_UNIT_0,
        MCPWM_TIMER_0,
        MOTOR_OPERATOR(motor),
        pulse_norm_us
    );
};


void esc_set_pwms(pwm_pulse_norm_t pulses_us[NUMBER_OF_MOTORS]) {
    pwm_pulse_norm_t throttle_us = pulses_us[0];
    pwm_pulse_norm_t steering_us = pulses_us[1];

    esc_set_pwm((pwm_pulse_t) throttle_us + steering_us - PWM_NEUTRAL_US, MOTOR_L);
    esc_set_pwm((pwm_pulse_t) throttle_us - steering_us + PWM_NEUTRAL_US, MOTOR_R);
};


void esc_validate() {

};
