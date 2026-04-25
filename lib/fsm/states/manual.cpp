#include "fsm.h"
#include "led.h"
#include "manual.h"
#include "motor.h"
#include "radio.h"

void manual_entry(void) {
    led_set_color(LED_STATE, COLOR_GREEN);
}

void manual_run(void) {
    if (radio_status() == RADIO_DISCONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }

    pwm_pulse_norm_t pulses_us[NUMBER_OF_MOTORS] = {
        radio_read_channel(CHANNEL_STEERING),
        radio_read_channel(CHANNEL_THROTTLE)
    };

    motors_set_pwm(pulses_us);
}

void manual_exit(void) {
    motors_set_pwm_neutral();
}
