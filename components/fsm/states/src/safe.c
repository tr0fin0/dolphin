#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "ir.h"
#include "opening.h"
#include "radio.h"
#include "safe.h"

extern uint8_t opening_step;

void safe_entry(void) {
    esc_set_pwm_mix_neutral();
}

void safe_run(void) {
    switch (CONFIG_CONTROL_MODE) {
        case CONFIG_CONTROL_AUTONOMOUS:
            if (ir_get_state() == IR_STATE_START) {
                fsm_transition(STATE_SEARCH);
            }
            break;

        case CONFIG_CONTROL_RADIO:
            if (radio_get_status() == RADIO_CONNECTED) {
                if (opening_step == OPENING_ITERATIONS) {
                    fsm_transition(STATE_MANUAL);
                } else {
                    fsm_transition(STATE_OPENING);
                }
            }
            break;

        default:
            break;
    }
}
