#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "opening.h"
#include "radio.h"
#include "safe.h"

extern opening_step_t opening_step;

void safe_entry(void) {
    esc_set_pwm_mix_neutral();
}

void safe_run(void) {
    if (radio_status() == RADIO_CONNECTED) {
        if (CONFIG_CONTROL_MODE == CONFIG_CONTROL_AUTONOMOUS) {
            fsm_transition(STATE_COUNTDOWN);
        } else {
            if (opening_step >= OPENING_ITERATIONS) {
                fsm_transition(STATE_MANUAL);
            } else {
                fsm_transition(STATE_OPENING);
            }
        }
    }
}
