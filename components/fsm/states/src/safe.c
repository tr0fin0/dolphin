#include "config.h"
#include "controller.h"
#include "fsm.h"
#include "ir.h"
#include "opening.h"
#include "radio.h"
#include "safe.h"

void safe_entry(void) {
    controller_stop();
}

void safe_run(void) {
    switch (CONFIG_CONTROL_MODE) {
        case CONFIG_CONTROL_AUTONOMOUS:
            if (opening_get_state() == OPENING_STATE_FINISHED) {
                if (ir_get_state() == IR_STATE_START) {
                    fsm_transition(FSM_STATE_AUTONOMOUS);
                }
            } else {
                if (radio_get_status() == RADIO_STATUS_CONNECTED) {
                    if (ir_get_state() == IR_STATE_STANDBY) {
                        fsm_transition(FSM_STATE_OPENING);
                    }
                }
            }
            break;

        case CONFIG_CONTROL_RADIO:
            if (radio_get_status() == RADIO_STATUS_CONNECTED) {
                if (opening_get_state() == OPENING_STATE_FINISHED) {
                    fsm_transition(FSM_STATE_MANUAL);
                } else {
                    fsm_transition(FSM_STATE_OPENING);
                }
            }
            break;

        default:
            break;
    }
}
