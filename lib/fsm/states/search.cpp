#include "debug.h"
#include "fsm.h"
#include "led.h"
#include "radio.h"



void search_entry(void) {
    led_set_color(LED_STATE, COLOR_BLUE);

    // TODO: Set ESCs to perform a slow spin or search pattern
}


void search_run(void) {
    // Safety abort
    if (radio_status() != RADIO_CONNECTED) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    // TODO: Read edge sensors -> if edge detected: fsm_transition(STATE_SURVIVE);
    // TODO: Read distance sensors -> if opponent seen: fsm_transition(STATE_ATTACK);
}
