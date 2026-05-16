#include "attack.h"
#include "colors.h"
#include "esc.h"
#include "fsm.h"
#include "led.h"
#include "radio.h"

void attack_entry(void) {
    led_set_color(LED_STATE, LED_COLOR_SCARLET);
}

void attack_run(void) {
    if (radio_status() != RADIO_CONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }
}

void attack_exit(void) {
    esc_set_pwm_mix_neutral();
}
