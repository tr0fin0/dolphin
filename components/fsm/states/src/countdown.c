#include "countdown.h"
#include "fsm.h"
#include "radio.h"

/**
 * @brief Entry countdown timestamp reference in milliseconds.
 */
// static uint16_t countdown_ms = 0;

void countdown_run(void) {
    if (radio_get_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}
