#include "attack.h"
#include "esc.h"
#include "fsm.h"
#include "radio.h"

void attack_run(void) {
    if (radio_get_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}

void attack_exit(void) {
    esc_set_pwm_mix_neutral();
}
