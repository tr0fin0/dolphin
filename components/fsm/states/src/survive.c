#include "fsm.h"
#include "radio.h"
#include "survive.h"

void survive_run(void) {
    if (radio_get_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}
