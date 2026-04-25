#include "boot.h"
#include "fsm.h"
#include "led.h"

void boot_entry(void) {
    led_set_color(LED_STATE, COLOR_WHITE);
}

void boot_run(void) {
    fsm_transition(STATE_SAFE);
}
