/**
 * @file fsm.h
 * @brief Table-driven finite-state machine API for sumo robot control.
 *
 * Provides a robust architecture with Entry, Run, and Exit actions
 * for strict state management and safety interlocks.
 */

#ifndef __FSM_H__
#define __FSM_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Define the core states of the sumo robot.
 */
typedef enum {
    STATE_ATTACK = 0,
    STATE_BOOT,
    STATE_COUNTDOWN,
    STATE_MANUAL,
    STATE_SAFE,
    STATE_SEARCH,
    STATE_SURVIVE,
    NUMBER_OF_STATES
} fsm_state_t;

typedef void (*fsm_action_t)(void);

/**
 * @brief Finite State Machine State Table.
 *
 * Defines the behaviour of a single FSM state using three optional
 * lifecycle callbacks:
 *
 * - on_entry: Executed once immediately after a transition INTO this state.
 *             Intended for initialization logic (e.g., reset counters,
 *             configure peripherals, set LEDs).
 *
 * - on_run:   Executed repeatedly while the FSM remains in this state.
 *             This function must be non-blocking and return quickly,
 *             as it is typically called from the main control loop.
 *
 * - on_exit:  Executed once immediately before transitioning OUT of
 *             this state.
 *             Intedended for cleanup or safe shutdown actions.
 *
 * Any callback may be `NULL` if not required.
 *
 * @note
 * - callbacks must be deterministic and non-blocking.
 * - transitions are managed by the FSM engine.
 */
typedef struct {
    const char* name;       /**< Human-readable state name (null-terminated). */
    fsm_action_t on_entry;  /**< Called once when entering the state. */
    fsm_action_t on_run;    /**< Called repeatedly while active. */
    fsm_action_t on_exit;   /**< Called once when leaving the state. */
} fsm_state_table_t;


/**
 * @brief Initialize the FSM and underlying hardware abstractions.
 */
void fsm_init(void);

/**
 * @brief Run one FSM step.
 */
void fsm_step(void);

/**
 * @brief Force a transition to a new FSM state.
 *
 * @param new_state The state to transition into.
 */
void fsm_transition(fsm_state_t new_state);

/**
 * @brief Get current state name as a null-terminated string.
 */
const char* get_current_state_name(void);

/**
 * @brief Get the current FSM state.
 */
fsm_state_t get_current_state(void);



#ifdef __cplusplus
}
#endif

#endif /* __FSM_H__ */
