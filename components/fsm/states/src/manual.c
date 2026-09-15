#include "esc.h"
#include "fsm.h"
#include "manual.h"
#include "radio.h"

void manual_exit(void) {
    esc_set_pwm_mix_neutral();
}

void manual_run(void) {
    if (radio_get_status() == RADIO_DISCONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }

    pwm_norm_t pulses_us[NUMBER_OF_ESCS] = {
        radio_read_channel(RADIO_CHANNEL_1),
        radio_read_channel(RADIO_CHANNEL_2)
    };

    esc_set_pwm_mix(pulses_us);
}
