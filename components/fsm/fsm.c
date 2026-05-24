#include "colors.h"
#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "led.h"
#include "logging.h"
#include "pwm.h"
#include "radio.h"
#include "states/include/attack.h"
#include "states/include/boot.h"
#include "states/include/countdown.h"
#include "states/include/manual.h"
#include "states/include/opening.h"
#include "states/include/safe.h"
#include "states/include/search.h"
#include "states/include/survive.h"

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
        .name       = "ATTACK",
        .color      = LED_COLOR_SCARLET,
        .on_entry   = NULL,
        .on_run     = attack_run,
        .on_exit    = attack_exit
    },
    [STATE_BOOT] = {
        .name       = "BOOT",
        .color      = LED_COLOR_WHITE,
        .on_entry   = NULL,
        .on_run     = boot_run,
        .on_exit    = NULL
    },
    [STATE_COUNTDOWN] = {
        .name       = "COUNTDOWN",
        .color      = LED_COLOR_ORANGE_LIGHT,
        .on_entry   = NULL,
        .on_run     = countdown_run,
        .on_exit    = NULL
    },
    [STATE_MANUAL] = {
        .name       = "MANUAL",
        .color      = LED_COLOR_GREEN,
        .on_entry   = NULL,
        .on_run     = manual_run,
        .on_exit    = manual_exit
    },
    [STATE_OPENING] = {
        .name       = "OPENING",
        .color      = LED_COLOR_PURPLE,
        .on_entry   = opening_entry,
        .on_run     = opening_run,
        .on_exit    = NULL,
    },
    [STATE_SAFE] = {
        .name       = "SAFE",
        .color      = LED_COLOR_RED,
        .on_entry   = safe_entry,
        .on_run     = safe_run,
        .on_exit    = NULL
    },
    [STATE_SEARCH] = {
        .name       = "SEARCH",
        .color      = LED_COLOR_BLUE,
        .on_entry   = NULL,
        .on_run     = search_run,
        .on_exit    = NULL
    },
    [STATE_SURVIVE] = {
        .name       = "SURVIVE",
        .color      = LED_COLOR_CYAN,
        .on_entry   = NULL,
        .on_run     = survive_run,
        .on_exit    = NULL
    }
};

void fsm_init(void) {
    esc_init();
    radio_init();

    current_state = STATE_BOOT;
}

void fsm_transition(fsm_state_t new_state) {
    // 0. skip invalid transitions or self-transitions
    if (current_state == new_state || new_state >= NUMBER_OF_STATES) {
        LOG_W("unkown state %d", new_state);

        return;
    }
    LOG_I(
        "transition from %s to %s",
        fsm_get_state_name(current_state),
        fsm_get_state_name(new_state)
    );

    // 1. execute Exit Action of current state
    if (fsm_table[current_state].on_exit != NULL) {
        LOG_D("exiting of %s", fsm_get_state_name(current_state));

        fsm_table[current_state].on_exit();
    }

    // 2. update state
    current_state = new_state;

    // 3. execute Entry Action of new state
    if (fsm_table[current_state].on_entry != NULL) {
        LOG_D("entrying of %s", fsm_get_state_name(current_state));

        fsm_table[current_state].on_entry();
    }

    // 4. set state color
    led_set_color(LED_STATE, fsm_table[current_state].color);
}

void fsm_step(void) {
    if (fsm_table[current_state].on_run == NULL) {
        LOG_E("no on_run() function for %s", fsm_get_state_name(current_state));

        return;
    }

    return fsm_table[current_state].on_run();
}

const char *fsm_get_state_name(fsm_state_t state) {
    if (fsm_table[state].name == NULL) {
        LOG_E("no name for %d", state);

        return "UNKNOWN";
    }

    return fsm_table[state].name;
}

fsm_state_t fsm_get_state(void) {
    return current_state;
}
