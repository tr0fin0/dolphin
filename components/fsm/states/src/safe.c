#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "ir.h"
#include "opening.h"
#include "radio.h"
#include "safe.h"

void safe_entry(void) {
    esc_set_pwm_mix_neutral();
}

void safe_run(void) {
    switch (CONFIG_CONTROL_MODE) {
        case CONFIG_CONTROL_AUTONOMOUS:
            if (opening_get_status() == OPENING_STATE_FINISHED) {
                if (ir_get_state() == IR_STATE_START) {
                    fsm_transition(STATE_AUTONOMOUS);
                }
            } else {
                if (ir_get_state() == IR_STATE_STANDBY) {
                    fsm_transition(STATE_OPENING);
                }
            }
            break;

        case CONFIG_CONTROL_RADIO:
            if (radio_get_status() == RADIO_CONNECTED) {
                if (opening_get_status() == OPENING_STATE_FINISHED) {
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
