#include <Arduino.h>
#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "led.h"
#include "motor.h"
#include "opening.h"
#include "pwm.h"
#include "radio.h"


static opening_t opening_strategy;
static pwm_pulse_norm_t button_us = 0;

uint8_t opening_step = 0;

void opening_entry(void) {
    led_set_color(LED_STATE, COLOR_PURPLE);

    button_us = radio_read_channel(CHANNEL_BUTTON);
}

void opening_run(void) {
    if (radio_status() == RADIO_DISCONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }

    pwm_pulse_norm_t pulses_us[NUMBER_OF_CHANNELS];
    radio_read_channels(pulses_us);

    // ensure inital button value is not PWM_NEUTRAL_US
    if (
        button_us == PWM_NEUTRAL_US &&
        pulses_us[CHANNEL_BUTTON] != PWM_NEUTRAL_US
    ) {
        button_us = pulses_us[CHANNEL_BUTTON];
    }

    if (
        opening_step < OPENING_ITERATIONS &&
        button_us != pulses_us[CHANNEL_BUTTON]
    ) {
        // opening selection
        button_us = pulses_us[CHANNEL_BUTTON];

        uint8_t increase;
        if (pulses_us[CHANNEL_THROTTLE] > PWM_PERCENTAGE_P50) {
            increase = 2;
        } else if (pulses_us[CHANNEL_THROTTLE] < PWM_PERCENTAGE_M50) {
            increase = 1;
        } else {
            increase = 0;
        }

        opening_strategy = (opening_t) (10 * opening_strategy + increase);
        LOG_W("current opening: %d", opening_strategy);

        led_toggle(LED_STATE);
        opening_step++;
    } else if (
        opening_step == OPENING_ITERATIONS &&
        button_us != pulses_us[CHANNEL_BUTTON]
    ) {
        // opening execution
        button_us = pulses_us[CHANNEL_BUTTON];
        LOG_W("running opening: %d", opening_strategy);

        switch (opening_strategy) {
            case OPENING_STATIC:
                break;

            case OPENING_DRAW:
                // rotation     +180
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(80);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_NE:
                // rotation     +045
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(25);
                motors_set_pwm_neutral();
                delay(1);

                // translation  +50
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(140);
                motors_set_pwm_neutral();
                delay(1);

                // rotation     -090
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(65);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_NN:
                // translation  +50
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(140);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_NW:
                // rotation     -045
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(25);
                motors_set_pwm_neutral();
                delay(1);

                // translation  +50
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(140);
                motors_set_pwm_neutral();
                delay(1);

                // rotation     +090
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(65);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_SE:
                // rotation     -045
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(30);
                motors_set_pwm_neutral();
                delay(1);

                // translation  -50
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(120);
                motors_set_pwm_neutral();
                delay(1);

                // rotation     +090
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(60);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_SEN:
                // rotation     -045
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(30);
                motors_set_pwm_neutral();
                delay(1);

                // translation  -50
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(120);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_SS:
                // translation  -50
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(120);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_SW:
                // rotation     +045
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(30);
                motors_set_pwm_neutral();
                delay(1);

                // translation  -50
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(120);
                motors_set_pwm_neutral();
                delay(1);

                // rotation     -090
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_R);
                delay(60);
                motors_set_pwm_neutral();
                delay(1);

                break;

            case OPENING_SWN:
                // rotation     +045
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(20);
                motors_set_pwm_neutral();
                delay(1);

                // translation  -50
                esc_set_pwm(pwm_pulse_percentage(-90), MOTOR_L);
                esc_set_pwm(pwm_pulse_percentage(+90), MOTOR_R);
                delay(120);
                motors_set_pwm_neutral();
                delay(1);

                break;
        }

        fsm_transition(STATE_MANUAL);
    }
}
