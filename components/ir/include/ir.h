/**
 * @file ir.h
 * @brief Infrared receiver device.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-06
 */

#pragma once

#include "driver/rmt_rx.h"
#include "pinout.h"
#include "protocol.h"
#include <stdint.h>

/**
 * @brief Infrared states.
 *
 * @note The @ref protocol_command_name command names must match the
 * defined @ref ir_state infrared states sequence.
 */
typedef enum ir_state {
    IR_STATE_READY= 0,  /**< Infrared receiver received the ready command. */
    IR_STATE_START,     /**< Infrared receiver received the start command. */
    IR_STATE_STOP,      /**< Infrared receiver received the stop command . */
    IR_STATE_WAIT,      /**< Infrared receiver waiting commands. */
    NUMBER_OF_IR_STATES /**< Number of infrared states. */
} ir_state_t;

/**
 * @brief Infrared receive callback function.
 *
 * @param[in] command Protocol command.
 */
typedef void (*ir_rx_callback_t)(const protocol_command_t *command);

/**
 * @brief Infrared receiver device.
 */
typedef struct ir {
    const char *name;                               /**< Human-readable null-terminated infrared name. */
    const char *state_names[NUMBER_OF_IR_STATES];   /**< Human-readable null-terminated Radio status. */
    const protocol_t *protocol;                     /**< Infrared decoding protocol. */
    volatile bool rx_done;                          /**< Infrared flag indicating a received frame. */
    volatile bool valid_data;                       /**< Infrared flag indicating successful decode. */
    bool active_low;                                /**< Infrared input inversion flag. */
    pin_t pin;                                      /**< Infrared pin connection. */
    ir_rx_callback_t callback;                      /**< Infrared receive function callback. */
    ir_state_t state;                               /**< Infrared state. */
    protocol_command_t decoded_command;             /**< Infrared decoded command buffer. */
    rmt_channel_handle_t rx_channel;                /**< Infrared RMT receive channel. */
    rmt_symbol_word_t rx_buffer[64];                /**< Infrared RMT receive buffer. */
} ir_t;

/**
 * @def IR_RMT_CLOCK_RESOLUTION_HZ
 * @brief RMT clock resolution in hertz.
 *
 * **Default Value:** 1 MHz, i.e., 1 tick = 1 µs
 */
#define IR_RMT_CLOCK_RESOLUTION_HZ  1000000

/**
 * @def IR_RMT_MEMORY_BLOCK_SYMBOL
 * @brief RMT memory block symbol size.
 *
 * **Default Value:** 64
 */
#define IR_RMT_MEMORY_BLOCK_SYMBOL  64

/**
 * @def IR_RMT_SIGNAL_MAX_NS
 * @brief RMT signal maximum duration in nano-seconds.
 *
 * **Default Value:** 12 ms
 */
#define IR_RMT_SIGNAL_MAX_NS 12000000

/**
 * @def IR_RMT_SIGNAL_MIN_NS
 * @brief RMT signal minimum duration in nano-seconds.
 *
 * **Default Value:** 1.25 us
 */
#define IR_RMT_SIGNAL_MIN_NS 1250

/**
 * @brief Returns the infrared name.
 *
 * @return Human-readable null-terminated string representing the name.
 */
const char *ir_get_name(void);

/**
 * @brief Returns the infrared current state.
 *
 * @return Current infrared state.
 */
ir_state_t ir_get_state(void);

/**
 * @brief Returns the infrared current state name.
 *
 * @return Human-readable null-terminated string representing the state name.
 */
const char *ir_get_state_name(void);

/**
 * @brief Initializes the infrared RMT RX channel.
 */
void ir_init(void);

/**
 * @brief Polls the infrared receiver state.
 *
 * Must be called continuously.
 */
void ir_poll(void);

/**
 * @brief Updates the infrared current state.
 *
 * @param[in] state Desired infrared state.
 */
void ir_set_state(ir_state_t state);
