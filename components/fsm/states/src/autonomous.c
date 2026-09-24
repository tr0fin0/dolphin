#include "autonomous.h"
#include "fsm.h"
#include "ir.h"

void autonomous_exit(void) {

}

void autonomous_run(void) {
    if (ir_get_state() != IR_STATE_START) {
        fsm_transition(STATE_SAFE);

        return;
    }
}