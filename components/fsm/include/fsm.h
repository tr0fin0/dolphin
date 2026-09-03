/**
 * @file fsm.h
 * @brief Finite State Machine API definition.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "led.h"

/**
 * @brief Finite State Machine states.
 *
 * Each state handler callback functions are defined in an independent `.c` and
 * `.h` files under the states folder.
 */
typedef enum fsm_state {
    STATE_ATTACK = 0,   /**< In the @ref CONFIG_CONTROL_AUTONOMOUS, attacks adversary. */
    STATE_BOOT,         /**< FSM entry point. */
    STATE_COUNTDOWN,    /**< In the @ref CONFIG_CONTROL_AUTONOMOUS, waits 5 seconds. */
    STATE_MANUAL,       /**< In the @ref CONFIG_CONTROL_RADIO, receives radio signals and control motors. */
    STATE_OPENING,      /**< In the @ref CONFIG_CONTROL_RADIO, selects and execute an opening move. */
    STATE_SAFE,         /**< In either control mode, freezes the system for safety handle. */
    STATE_SEARCH,       /**< In the @ref CONFIG_CONTROL_AUTONOMOUS, aligns with adversary. */
    STATE_SURVIVE,      /**< In the @ref CONFIG_CONTROL_AUTONOMOUS, avoid leaving the dojo. */
    NUMBER_OF_STATES    /**< Number of FSM states currently implemented. */
} fsm_state_t;

/**
 * @brief Finite State Machine function pointer.
 */
typedef void (*fsm_action_t)(void);

/**
 * @brief Finite State Machine State table.
 *
 * Defines the behaviour of a single FSM state using three optional
 * lifecycle callbacks:
 *
 * - `on_entry`: Executed once immediately after a transition INTO this state.
 *
 * - `on_run`: Executed repeatedly while the FSM remains in this state.
 *
 * - `on_exit`:  Executed once immediately before a transition OUT OF this state.
 *
 * Any callback may be `NULL` if not required.
 *
 * @note All callbacks must be deterministic and non-blocking.
 */
typedef struct fsm_table {
    const char *name;       /**< Human-readable null-terminated state name. */
    led_color_t color;      /**< LED color set upon entering the state. */
    fsm_action_t on_entry;  /**< Called once when entering the state. */
    fsm_action_t on_run;    /**< Called repeatedly while active. */
    fsm_action_t on_exit;   /**< Called once when leaving the state. */
} fsm_table_t;

/**
 * @brief Get current FSM state enumerated value.
 *
 * @return Current FSM state enumerated value.
 */
fsm_state_t fsm_get_current_state(void);

/**
 * @brief Get the FSM state name as a null-terminated string.
 *
 * @param[in] state A FSM state.
 * @return Human-readable null-terminated FSM name.
 */
const char *fsm_get_state_name(fsm_state_t state);

/**
 * @brief Initialize the FSM and underlying hardware abstractions.
 */
void fsm_init(void);

/**
 * @brief Run one FSM step.
 *
 * Executes the `on_run` callback of the currently active state.
 */
void fsm_step(void);

/**
 * @brief Transition from current FSM state to a new FSM state.
 *
 * @param[in] new_state The state to transition into.
 */
void fsm_transition(fsm_state_t new_state);
