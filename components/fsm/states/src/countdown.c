#include "colors.h"
#include "countdown.h"
#include "fsm.h"
#include "led.h"
#include "radio.h"

/**
 * @brief Entry countdown timestamp reference in milliseconds.
 */
// static uint16_t countdown_ms = 0;

void countdown_entry(void) {
    led_set_color(LED_STATE, LED_COLOR_ORANGE_LIGHT);
}

void countdown_run(void) {
    if (radio_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}
