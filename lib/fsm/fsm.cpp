#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "led.h"
#include "pwm.h"
#include "radio.h"
#include "states/attack.h"
#include "states/boot.h"
#include "states/countdown.h"
#include "states/manual.h"
#include "states/opening.h"
#include "states/safe.h"
#include "states/search.h"
#include "states/survive.h"



/**
 * @brief Currently active FSM state.
 *
 * Holds the state in which the FSM is presently operating.
 *
 * @note
 * - initialized to `STATE_BOOT` during system startup.
 *
 * - modified only by the FSM transition logic.
 */
static fsm_state_t current_state = STATE_BOOT;

static const fsm_table_t fsm_table[NUMBER_OF_STATES] = {
    [STATE_ATTACK] = {
        .name     = "ATTACK",
        .on_entry = attack_entry,
        .on_run   = attack_run,
        .on_exit  = attack_exit
    },
    [STATE_BOOT] = {
        .name     = "BOOT",
        .on_entry = boot_entry,
        .on_run   = boot_run,
        .on_exit  = NULL
    },
    [STATE_COUNTDOWN] = {
        .name     = "COUNTDOWN",
        .on_entry = countdown_entry,
        .on_run   = countdown_run,
        .on_exit  = NULL
    },
    [STATE_MANUAL] = {
        .name     = "MANUAL",
        .on_entry = manual_entry,
        .on_run   = manual_run,
        .on_exit  = manual_exit
    },
    [STATE_OPENING] = {
        .name     = "OPENING",
        .on_entry = opening_entry,
        .on_run   = opening_run,
        .on_exit  = NULL,
    },
    [STATE_SAFE] = {
        .name     = "SAFE",
        .on_entry = safe_entry,
        .on_run   = safe_run,
        .on_exit  = NULL
    },
    [STATE_SEARCH] = {
        .name     = "SEARCH",
        .on_entry = search_entry,
        .on_run   = search_run,
        .on_exit  = NULL
    },
    [STATE_SURVIVE] = {
        .name     = "SURVIVE",
        .on_entry = survive_entry,
        .on_run   = survive_run,
        .on_exit  = NULL
    }
};

void fsm_init(void) {
    LOG_I("initialization Finite State Machine started");

    led_init();
    led_set_brightness(BRIGHTNESS_MEDIUM);
    radio_init();
    esc_init();

    current_state = STATE_BOOT;

    LOG_I("initialization Finite State Machine finish");
}

void fsm_transition(fsm_state_t new_state) {
    // 0. skip invalid transitions or self-transitions
    if (current_state == new_state || new_state >= NUMBER_OF_STATES) {
        LOG_W("unkown state %d", new_state);

        return;
    }
    LOG_I(
        "transition from current state %s to %s",
        fsm_get_state_name(current_state),
        fsm_get_state_name(new_state)
    );

    // 1. execute Exit Action of current state
    if (fsm_table[current_state].on_exit != NULL) {
        LOG_I("exiting of %s", fsm_get_state_name(current_state));

        fsm_table[current_state].on_exit();
    }

    // 2. update state
    current_state = new_state;

    // 3. execute Entry Action of new state
    if (fsm_table[current_state].on_entry != NULL) {
        LOG_I("entrying of %s", fsm_get_state_name(current_state));

        fsm_table[current_state].on_entry();
    }
}

void fsm_step(void) {
    if (fsm_table[current_state].on_run != NULL) {
        LOG_I("running of %s", fsm_get_state_name(current_state));

        return fsm_table[current_state].on_run();
    }

    LOG_E(
        "no on_run() function defined for %s", fsm_get_state_name(current_state)
    );
}

const char *fsm_get_state_name(fsm_state_t state) {
    if (fsm_table[state].name != NULL) {
        return fsm_table[state].name;
    }

    LOG_W("no name defined for state %d", state);

    return "UNKNOWN";
}

fsm_state_t fsm_get_state(void) {
    return current_state;
}
