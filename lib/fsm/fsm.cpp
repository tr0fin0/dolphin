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



// -----------------------------------------------------------------------------
// FSM State Variables
// -----------------------------------------------------------------------------

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
static fsm_state_t  current_state   = STATE_BOOT;



// -----------------------------------------------------------------------------
// FSM State Table Definition
// -----------------------------------------------------------------------------

static const fsm_state_table_t state_table[NUMBER_OF_STATES] = {
    [STATE_ATTACK] = {
        .name     = "ATTACK",
        .on_entry = attack_entry,
        .on_run   = attack_run,
        .on_exit  = NULL
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



// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

void fsm_init(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization Finite State Machine started");

    led_init();
    led_set_brightness(BRIGHTNESS_MEDIUM);
    radio_init();
    esc_init();

    current_state = STATE_BOOT;

    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization Finite State Machine finish");
}


void fsm_transition(fsm_state_t new_state) {
    // 0. skip invalid transitions or self-transitions
    if (current_state == new_state || new_state >= NUMBER_OF_STATES) {
        DEBUG_MSG(
            DEBUG_LEVEL_WARNING,
            state_table[current_state].name,
            (new_state < NUMBER_OF_STATES)
                ? state_table[new_state].name
                : "INVALID"
        );

        return;
    }
    DEBUG_MSG(
        DEBUG_LEVEL_INFO,
        "transition from current state %s to %s",
        state_table[current_state].name,
        (new_state < NUMBER_OF_STATES)
            ? state_table[new_state].name
            : "INVALID"
    );

    // 1. execute Exit Action of current state
    if (state_table[current_state].on_exit != NULL) {
        DEBUG_MSG(
            DEBUG_LEVEL_INFO, "exiting of %s", state_table[current_state].name
        );

        state_table[current_state].on_exit();
    }

    // 2. update state
    current_state = new_state;

    // 3. execute Entry Action of new state
    if (state_table[current_state].on_entry != NULL) {
        DEBUG_MSG(
            DEBUG_LEVEL_INFO, "entrying of %s", state_table[current_state].name
        );

        state_table[current_state].on_entry();
    }
}


void fsm_step(void) {
    if (state_table[current_state].on_run != NULL) {
        DEBUG_MSG(
            DEBUG_LEVEL_INFO, "running of %s", state_table[current_state].name
        );

        return state_table[current_state].on_run();
    }

    DEBUG_MSG(
        DEBUG_LEVEL_CRITICAL,
        "no on_run() function defined for current state %d", current_state
    );
}


const char* get_current_state_name(void) {
    if (state_table[current_state].name != NULL) {
        return state_table[current_state].name;
    }

    DEBUG_MSG(
        DEBUG_LEVEL_ERROR, "no name defined for current state %d", current_state
    );

    return "UNKNOWN";
}


fsm_state_t get_current_state(void) {
    return current_state;
}
