#include "config.h"
#include "debug.h"
#include "fsm.h"
#include "led.h"
#include "radio.h"



/**
 * @brief State timing reference in milliseconds.
 */
static uint32_t state_timer_ms      = 0;



void countdown_entry(void) {
    led_set_color(LED_STATE, COLOR_ORANGE_LIGHT);
    // state_timer_ms = millis();

    DEBUG_MSG(DEBUG_LEVEL_INFO, "countdown initiated");
}


void countdown_run(void) {
    // Safety abort
    if (radio_status() != RADIO_CONNECTED) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    // 5-second Japanese sumo rule
    // if (millis() - state_timer_ms >= COUNTDOWN_MS) {
    //     DEBUG_MSG(DEBUG_LEVEL_INFO, "countdown terminated");
    //     fsm_transition(STATE_SEARCH);
    // }
}
