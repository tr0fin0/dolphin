#include "config.h"
#include "fsm.h"
#include "led.h"
#include "motor.h"
#include "opening.h"
#include "radio.h"
#include "safe.h"

extern uint8_t opening_step;

void safe_entry(void) {
    led_set_color(LED_STATE, COLOR_RED);

    motors_set_pwm_neutral();
}

void safe_run(void) {
    if (radio_status() == RADIO_CONNECTED) {
        LOG_I("control mode: %d", CONTROL_MODE);

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
