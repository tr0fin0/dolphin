#include "debug.h"
#include "fsm.h"
#include "led.h"
#include "radio.h"
#include "attack.h"



void attack_entry(void) {
    led_set_color(LED_STATE, COLOR_SCARLET);

    // TODO: Set ESCs to full forward speed
}


void attack_run(void) {
    // Safety abort
    if (radio_status() != RADIO_CONNECTED) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    // TODO: Read edge sensors -> if edge detected: fsm_transition(STATE_SURVIVE);
    // TODO: Read distance sensors -> if opponent lost: fsm_transition(STATE_SEARCH);
}