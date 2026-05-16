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

/**
 * @brief FSM possible states.
 *
 * Each state handler callback functions are defined in an independent `.c` and
 * `.h` files under the states folder.
 */
typedef enum {
    /** In the Autonomous Control, attacks adversary. */
    STATE_ATTACK = 0,
    /** Entry point of the FSM. */
    STATE_BOOT,
    /** In the Autonomous Control, waits 5 seconds. */
    STATE_COUNTDOWN,
    /** In the Radio Control, receives radio signals and control motors. */
    STATE_MANUAL,
    /** In the Radio Control, selects and execute an opening move. */
    STATE_OPENING,
    /** In the either control, freezes the system for safety handle. */
    STATE_SAFE,
    /** In the Autonomous Control, aligns with adversary. */
    STATE_SEARCH,
    /** In the Autonomous Control, avoid leaving the dojo. */
    STATE_SURVIVE,
    /** Quantity of states currently implemented in the FSM. */
    NUMBER_OF_STATES
} fsm_state_t;

/**
 * @brief FSM function pointer.
 */
typedef void (*fsm_action_t)(void);

/**
 * @brief Finite State Machine State Table.
 *
 * Defines the behaviour of a single FSM state using three optional
 * lifecycle callbacks:
 *
 * - on_entry: Executed once immediately after a transition INTO this state.
 *
 * - on_run:   Executed repeatedly while the FSM remains in this state.
 *
 * - on_exit:  Executed once immediately before a transition OUT this state.
 *
 * Any callback may be `NULL` if not required.
 *
 * @note
 * - callbacks must be deterministic and non-blocking.
 */
typedef struct {
    const char *name;      /**< Human-readable null-terminated state name. */
    fsm_action_t on_entry; /**< Called once when entering the state. */
    fsm_action_t on_run;   /**< Called repeatedly while active. */
    fsm_action_t on_exit;  /**< Called once when leaving the state. */
} fsm_table_t;

/**
 * @brief Initialize the FSM and underlying hardware abstractions.
 */
void fsm_init(void);

/**
 * @brief Run one FSM step.
 */
void fsm_step(void);

/**
 * @brief Transition from current FSM state to a new FSM state.
 *
 * @param new_state The state to transition into.
 */
void fsm_transition(fsm_state_t new_state);

/**
 * @brief Get the FSM state name as a null-terminated string.
 *
 * @param state A FSM state.
 */
const char *fsm_get_state_name(fsm_state_t state);

/**
 * @brief Get current FSM state as a enumerate value.
 */
fsm_state_t fsm_get_state(void);
