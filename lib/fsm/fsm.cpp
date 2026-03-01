#include <Arduino.h>
#include "config.h"
#include "esc/esc.h"
#include "fsm.h"
#include "led/colors.h"
#include "led/led.h"
#include "radio/radio.h"
#include "pinout.h"
#include "pwm/pwm.h"



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

/**
 * @brief State timing reference in milliseconds.
 */
static uint32_t state_timer_ms      = 0;



// -----------------------------------------------------------------------------
// FSM State Functions
// -----------------------------------------------------------------------------

static void attack_entry(void);
static void attack_run(void);

static void boot_entry(void);
static void boot_run(void);

static void countdown_entry(void);
static void countdown_run(void);

static void manual_entry(void);
static void manual_run(void);
static void manual_exit(void);

static void search_entry(void);
static void search_run(void);

static void safe_entry(void);
static void safe_run(void);

static void survive_entry(void);
static void survive_run(void);

static void stop_motors(void);




// -----------------------------------------------------------------------------
// FSM State Table Definition
// -----------------------------------------------------------------------------

/**
 * @brief Mermaid FSM Diagram
 *
stateDiagram-v2
    classDef autonomous fill:#900
    classDef common     fill:#909
    classDef radio      fill:#009
    
    class STATE_ATTACK, STATE_COUNTDOWN, STATE_SEARCH autonomous
    class STATE_MANUAL radio
    class STATE_SAFE, STATE_SURVIVE common
 
    
    direction TB
    state IF_SAFE <<choice>>


    %% INITIALIZATION
    [*] --> STATE_SAFE:                 fsm_init()
    STATE_SAFE --> [*]


    %% COMMON

    STATE_SAFE --> IF_SAFE:             if radio_status() == RADIO_CONNECTED_ENABLE
    IF_SAFE --> STATE_COUNTDOWN:        if CONTROL_MODE == AUTONOMOUS
    IF_SAFE --> STATE_MANUAL:           if CONTROL_MODE == RADIO

    %%STATE_SURVIVE --> STATE_MANUAL:     if edge_status() != EDGE_DETECTED
    STATE_SURVIVE --> STATE_SEARCH:     if edge_status() != EDGE_DETECTED


    %% AUTONOMOUS
    STATE_ATTACK --> STATE_SURVIVE:     if edge_status() == EDGE_DETECTED

    STATE_COUNTDOWN --> STATE_SAFE:     if radio_status() != RADIO_CONNECTED_ENABLE
    STATE_COUNTDOWN --> STATE_SEARCH:   if millis() - state_timer_ms >= 5000

    STATE_SEARCH --> STATE_ATTACK:      if edge_status() != EDGE_DETECTED && if oponent_status() == OPONENT_DETECDED
    STATE_SEARCH --> STATE_SAFE:        if radio_status() != RADIO_CONNECTED_ENABLE
    STATE_SEARCH --> STATE_SURVIVE:     if edge_status() == EDGE_DETECTED


    %% RADIO CONTROLLED
    STATE_MANUAL --> STATE_SAFE:        if radio_status() != RADIO_CONNECTED_ENABLE
    %%STATE_MANUAL --> STATE_SURVIVE:     if edge_status() == EDGE_DETECTED
 */


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
        state_table[current_state].on_exit();
    }

    // 2. update state
    current_state = new_state;

    // 3. execute Entry Action of new state
    if (state_table[current_state].on_entry != NULL) {
        state_table[current_state].on_entry();
    }
}


void fsm_step(void) {
    if (state_table[current_state].on_run != NULL) {
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
        DEBUG_LEVEL_ERROR,
        "no name defined for current state %d", current_state
    );

    return "UNKNOWN";
}


fsm_state_t get_current_state(void) {
    return current_state;
}



// -----------------------------------------------------------------------------
// State Behaviors
// -----------------------------------------------------------------------------

/**
 * @brief Stop both motors by commanding neutral PWM.
 *
 * Sets left and right ESC outputs to `PWM_NEUTRAL_US`, ensuring zero throttle
 * condition.
 */
static void stop_motors(void) {
    DEBUG_MSG(
        DEBUG_LEVEL_INFO,
        "stopping motors"
    );

    esc_set_pwm((pwm_pulse_t) PWM_NEUTRAL_US, MOTOR_L);
    esc_set_pwm((pwm_pulse_t) PWM_NEUTRAL_US, MOTOR_R);
}


// --- ATTACK ---

/**
 * @brief Attack state entry handler.
 *
 * Sets visual indication upon entering `STATE_ATTACK`.
 */
static void attack_entry(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "entrying of STATE_ATTACK");

    led_set_color(LED_STATE, COLOR_SCARLET);

    // TODO: Set ESCs to full forward speed
}

/**
 * @brief Attack state run handler.
 *
 * Monitors radio enable signal and transitions to `STATE_SAFE` if control
 * authorization is lost.
 */
static void attack_run(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "running of STATE_ATTACK");

    // Safety abort
    if (radio_status() != RADIO_CONNECTED_ENABLE) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    // TODO: Read edge sensors -> if edge detected: fsm_transition(STATE_SURVIVE);
    // TODO: Read distance sensors -> if opponent lost: fsm_transition(STATE_SEARCH);
}


// --- BOOT ----


/**
 * @brief Boot state entry handler.
 *
 * Sets visual indication upon entering `STATE_BOOT`.
 */
static void boot_entry(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "entrying of STATE_BOOT");

    led_set_color(LED_STATE, COLOR_WHITE);
}

/**
 * @brief Boot state run handler.
 *
 * First .
 */
static void boot_run(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "running of STATE_BOOT");

    fsm_transition(STATE_SAFE);
}


// --- COUNTDOWN ---

/**
 * @brief Countdown state entry handler.
 *
 * Sets visual indication for countdown phase and captures the entry timestamp
 * for time-based transition.
 */
static void countdown_entry(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "entrying of STATE_COUNTDOWN");

    led_set_color(LED_STATE, COLOR_ORANGE_LIGHT);
    state_timer_ms = millis();

    DEBUG_MSG(DEBUG_LEVEL_INFO, "countdown initiated");
}

/**
 * @brief Countdown state run handler.
 *
 * Transitions to `STATE_SEARCH` after a 5-second delay from state entry. If the
 * radio enable signal is lost, immediately transitions back to `STATE_SAFE`.
 */
static void countdown_run(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "running of STATE_COUNTDOWN");

    // Safety abort
    if (radio_status() != RADIO_CONNECTED_ENABLE) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    // 5-second Japanese sumo rule
    if (millis() - state_timer_ms >= COUNTDOWN_MS) {
        DEBUG_MSG(DEBUG_LEVEL_INFO, "countdown terminated");
        fsm_transition(STATE_SEARCH);
    }
}


// --- MANUAL ---

/**
 * @brief Manual state entry handler.
 *
 * Sets visual indication upon entering `STATE_MANUAL`.
 */
static void manual_entry(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "entrying of STATE_MANUAL");

    led_set_color(LED_STATE, COLOR_GREEN);
}

/**
 * @brief Manual state run handler.
 *
 * In `RADIO_CONNECTED_ENABLE` state, reads steering and throttle radio channels
 * and forwards them directly to the ESC outputs.
 *
 * If the radio becomes disabled or disconnected, transitions immediately to
 * `STATE_SAFE` for safety.
 */
static void manual_run(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "running of STATE_MANUAL");

    if (radio_status_pin() == RADIO_DISCONNECTED) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "Radio signal lost (pulseIn timeout). Stopping motors.");

        fsm_transition(STATE_SAFE); 
        return;
    }

    pwm_pulse_norm_t steering_us = radio_read_pin(PIN_RADIO_CH1);
    pwm_pulse_norm_t throttle_us = radio_read_pin(PIN_RADIO_CH2);

    DEBUG_MSG(DEBUG_LEVEL_TRACE, "measured steering: %d us", steering_us);
    DEBUG_MSG(DEBUG_LEVEL_TRACE, "measured throttle: %d us", throttle_us);

    long left_us  = throttle_us + steering_us - PWM_NEUTRAL_US;
    long right_us = throttle_us - steering_us + PWM_NEUTRAL_US;

    esc_set_pwm((pwm_pulse_t) left_us,  MOTOR_L);
    esc_set_pwm((pwm_pulse_t) right_us, MOTOR_R);
}

/**
 * @brief Manual state exit handler.
 *
 * Ensures both motors are driven to neutral PWM when leaving `STATE_MANUAL`,
 * preventing unintended motion during transition.
 */
static void manual_exit(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "exiting of STATE_MANUAL");

    stop_motors();
}


// --- SAFE ---

/**
 * @brief Safe state entry handler.
 *
 * Activates visual warning indication and ensures all motors are stopped upon
 * entering the safe state.
 */
static void safe_entry(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "entring of STATE_SAFE");

    led_set_color(LED_STATE, COLOR_YELLOW);
    led_set_brightness(BRIGHTNESS_MEDIUM);

    stop_motors();
}

/**
 * @brief Safe state run handler.
 *
 * Monitors radio status and transitions to `STATE_COUNTDOWN` when a valid
 * enable signal is detected.
 */
static void safe_run(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "running of STATE_SAFE");

    if (radio_status_pin() == RADIO_CONNECTED_ENABLE) {
        DEBUG_MSG(DEBUG_LEVEL_INFO, "radio status is RADIO_CONNECTED_ENABLE");

        if (CONTROL_MODE == AUTONOMOUS) {
            DEBUG_MSG(DEBUG_LEVEL_INFO, "current control mode is AUTONOMOUS");

            fsm_transition(STATE_COUNTDOWN);
        } else {
            DEBUG_MSG(DEBUG_LEVEL_INFO, "current control mode is RADIO");

            fsm_transition(STATE_MANUAL);
        }
    }

    DEBUG_MSG(DEBUG_LEVEL_WARNING, "radio status is RADIO_DISCONNECTED");
}


// --- SEARCH ---

/**
 * @brief Search state entry handler.
 *
 * Sets visual indication upon entering `STATE_SEARCH`.
 */
static void search_entry(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "entring of STATE_SEARCH");

    led_set_color(LED_STATE, COLOR_BLUE);

    // TODO: Set ESCs to perform a slow spin or search pattern
}

/**
 * @brief Search state run handler.
 *
 * Continuously monitors radio status and transitions to `STATE_SAFE` if the
 * enable signal is lost.
 */
static void search_run(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "running of STATE_SEARCH");

    // Safety abort
    if (radio_status() != RADIO_CONNECTED_ENABLE) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    // TODO: Read edge sensors -> if edge detected: fsm_transition(STATE_SURVIVE);
    // TODO: Read distance sensors -> if opponent seen: fsm_transition(STATE_ATTACK);
}


// --- SURVIVE ---

/**
 * @brief Survive state entry handler.
 *
 * Sets visual indication once upon entering `STATE_SURVIVE` and captures the
 * entry timestamp for time-based logic.
 */
static void survive_entry(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "entring of STATE_SURVIVE");

    led_set_color(LED_STATE, COLOR_PURPLE);

    // TODO: Set ESCs to hard reverse
}

/**
 * @brief Survive state run handler.
 *
 * Maintains safety monitoring and executes a timed maneuver:
 *
 * - Immediately transitions to `STATE_SAFE` if radio enable is lost.
 *
 * - After 500 ms from state entry, transitions to `STATE_SEARCH`.
 */
static void survive_run(void) {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "running of STATE_SURVIVE");

    // Safety abort
    if (radio_status() != RADIO_CONNECTED_ENABLE) {
        DEBUG_MSG(DEBUG_LEVEL_WARNING, "unable to connect with radio");
        fsm_transition(STATE_SAFE);

        return;
    }

    fsm_transition(STATE_SEARCH);
}
