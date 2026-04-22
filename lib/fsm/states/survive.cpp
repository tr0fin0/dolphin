#include "debug.h"
#include "fsm.h"
#include "led.h"
#include "radio.h"



void survive_entry(void) {
    led_set_color(LED_STATE, COLOR_PURPLE);

    // TODO: Set ESCs to hard reverse
}


void survive_run(void) {
    // Safety abort
    if (radio_status() != RADIO_CONNECTED) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    fsm_transition(STATE_SEARCH);
}
