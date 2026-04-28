#include "fsm.h"
#include "led.h"
#include "radio.h"
#include "search.h"

void search_entry(void) {
    led_set_color(LED_STATE, COLOR_BLUE);
}

void search_run(void) {
    if (radio_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}
