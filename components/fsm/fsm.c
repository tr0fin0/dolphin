#include "esc.h"
#include "fsm.h"
#include "ir.h"
#include "led.h"
#include "logging.h"
#include "pwm.h"
#include "radio.h"
#include "sensor.h"
#include "states/include/attack.h"
#include "states/include/autonomous.h"
#include "states/include/boot.h"
#include "states/include/manual.h"
#include "states/include/opening.h"
#include "states/include/safe.h"
#include "states/include/search.h"
#include "states/include/survive.h"

static fsm_t fsm = {
    .name = "FSM",
    .state = FSM_STATE_BOOT,
    .states = {
        [FSM_STATE_ATTACK] = {
            .name       = "ATTACK",
            .color      = LED_COLOR_SCARLET,
            .on_entry   = attack_entry,
            .on_run     = attack_run,
            .on_exit    = attack_exit
        },
        [FSM_STATE_AUTONOMOUS] = {
            .name       = "AUTONOMOUS",
            .color      = LED_COLOR_ORANGE_DARK,
            .on_entry   = autonomous_entry,
            .on_run     = autonomous_run,
            .on_exit    = autonomous_exit
        },
        [FSM_STATE_BOOT] = {
            .name       = "BOOT",
            .color      = LED_COLOR_WHITE,
            .on_entry   = NULL,
            .on_run     = boot_run,
            .on_exit    = NULL
        },
        [FSM_STATE_MANUAL] = {
            .name       = "MANUAL",
            .color      = LED_COLOR_RED,
            .on_entry   = NULL,
            .on_run     = manual_run,
            .on_exit    = manual_exit
        },
        [FSM_STATE_OPENING] = {
            .name       = "OPENING",
            .color      = LED_COLOR_BLUE,
            .on_entry   = opening_entry,
            .on_run     = opening_run,
            .on_exit    = NULL,
        },
        [FSM_STATE_SAFE] = {
            .name       = "SAFE",
            .color      = LED_COLOR_GREEN,
            .on_entry   = safe_entry,
            .on_run     = safe_run,
            .on_exit    = NULL
        },
        [FSM_STATE_SEARCH] = {
            .name       = "SEARCH",
            .color      = LED_COLOR_PURPLE,
            .on_entry   = search_entry,
            .on_run     = search_run,
            .on_exit    = search_exit
        },
        [FSM_STATE_SURVIVE] = {
            .name       = "SURVIVE",
            .color      = LED_COLOR_CYAN,
            .on_entry   = survive_entry,
            .on_run     = survive_run,
            .on_exit    = survive_exit
        }
    },
};

fsm_state_t fsm_get_current_state(void) {
    return fsm.state;
}

const char *fsm_get_state_name(fsm_state_t state) {
    if (fsm.states[state].name == NULL) {
        LOG_E("no name for %02d", state);

        return "UNKNOWN";
    }

    return fsm.states[state].name;
}

void fsm_init(void) {
    esc_init();
    ir_init();
    radio_init();
    sensor_init();

    fsm.state = FSM_STATE_BOOT;
}

void fsm_step(void) {
    if (fsm.states[fsm.state].on_run == NULL) {
        LOG_E("no on_run() function for %s", fsm_get_state_name(fsm.state));

        return;
    }

    return fsm.states[fsm.state].on_run();
}

void fsm_transition(fsm_state_t new_state) {
    // 0. skip invalid transitions or self-transitions
    if (fsm.state == new_state || new_state >= NUMBER_OF_FSM_STATES) {
        LOG_W("unknown state %02d", new_state);

        return;
    }
    LOG_I(
        "transition from %s to %s",
        fsm_get_state_name(fsm.state),
        fsm_get_state_name(new_state)
    );

    // 1. execute Exit Action of current state
    if (fsm.states[fsm.state].on_exit != NULL) {
        LOG_D("exiting of %s", fsm_get_state_name(fsm.state));

        fsm.states[fsm.state].on_exit();
    }

    // 2. update state
    fsm.state = new_state;

    // 3. execute Entry Action of new state
    if (fsm.states[fsm.state].on_entry != NULL) {
        LOG_D("entrying of %s", fsm_get_state_name(fsm.state));

        fsm.states[fsm.state].on_entry();
    }

    // 4. set state color
    led_set_color(LED_STATE, fsm.states[fsm.state].color);
}
