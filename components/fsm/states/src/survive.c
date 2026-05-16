#include "colors.h"
#include "fsm.h"
#include "led.h"
#include "radio.h"
#include "survive.h"

void survive_entry(void) {
    led_set_color(LED_STATE, LED_COLOR_PURPLE);
}

void survive_run(void) {
    if (radio_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}
