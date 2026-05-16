#include "colors.h"
#include "esc.h"
#include "fsm.h"
#include "led.h"
#include "manual.h"
#include "radio.h"

void manual_entry(void) {
    led_set_color(LED_STATE, LED_COLOR_GREEN);
}

void manual_run(void) {
    if (radio_status() == RADIO_DISCONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }

    pwm_norm_t pulses_us[NUMBER_OF_ESCS] = {
        radio_read_channel(CHANNEL_STEERING),
        radio_read_channel(CHANNEL_THROTTLE)
    };

    esc_set_pwm_mix(pulses_us);
}

void manual_exit(void) {
    esc_set_pwm_mix_neutral();
}
