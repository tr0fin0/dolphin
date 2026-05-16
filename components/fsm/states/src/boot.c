#include "boot.h"
#include "colors.h"
#include "fsm.h"
#include "led.h"

void boot_entry(void) {
    led_set_color(LED_STATE, LED_COLOR_WHITE);
}

void boot_run(void) {
    fsm_transition(STATE_SAFE);
}
