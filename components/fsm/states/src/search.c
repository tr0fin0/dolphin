#include "fsm.h"
#include "radio.h"
#include "search.h"

void search_run(void) {
    if (radio_get_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}
