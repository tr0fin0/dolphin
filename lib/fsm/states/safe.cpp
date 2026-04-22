#include "config.h"
#include "fsm.h"
#include "led.h"
#include "motor.h"
#include "radio.h"



extern uint32_t opening_step;


void safe_entry(void) {
    led_set_color(LED_STATE, COLOR_RED);

    motors_set_pwm_neutral();
}


void safe_run(void) {
    // Safety lock
    if (radio_status() == RADIO_CONNECTED) {
        DEBUG_MSG(DEBUG_LEVEL_INFO, "control mode: %d", CONTROL_MODE);

        if (CONTROL_MODE == AUTONOMOUS) {
            fsm_transition(STATE_COUNTDOWN);
        } else {
            if (opening_step == OPENING_ITERATIONS) {
                fsm_transition(STATE_MANUAL);
            } else {
                fsm_transition(STATE_OPENING);
            }
        }
    }
}
