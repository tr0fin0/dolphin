/**
 * @file protocol.h
 * @brief Infrared receiver protocols definition.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-06
 */

#pragma once

#include "driver/rmt_types.h"
#include <stdint.h>

/**
 * @brief Supported infrared protocols names.
 */
typedef enum protocol_name {
    PROTOCOL_SONY = 0,          /**< Sony Serial Infra-Red Control protocol. */
    NUMBER_OF_PROTOCOL_NAMES    /**< Number of protocol names. */
} protocol_name_t;

/**
 * @brief Supported infrared protocols names.
 *
 * @note The @ref protocol_command_name command names must match the
 * defined @ref ir_state infrared states sequence.
 */
typedef enum protocol_command_name {
    PROTOCOL_COMMAND_READY = 0, /**< Protocol command for infrared ready state. */
    PROTOCOL_COMMAND_START,     /**< Protocol command for infrared start state. */
    PROTOCOL_COMMAND_STOP,      /**< Protocol command for infrared stop state. */
    PROTOCOL_COMMAND_WAIT,      /**< Protocol command for infrared wait state. */
    NUMBER_OF_PROTOCOL_COMMANDS /**< Number of protocol commands. */
} protocol_command_name_t;

/**
 * @brief Infrared protocol command.
 */
typedef struct protocol_command {
    uint8_t bit_count;          /**< Command payload in bits. */
    uint32_t address;           /**< Extracted address bits. */
    uint32_t command;           /**< Extracted command bits. */
    protocol_name_t protocol;   /**< Protocol detected. */
} protocol_command_t;

/**
 * @brief Function pointer interface for protocol decode.
 *
 * @param[in] symbols Array of RMT symbols.
 * @param[in] num_symbols Number of RMT symbols in the array.
 * @param[out] command Pointer to structure where data will be stored.
 *
 * @return true if decoding succeeded, false otherwise.
 */
typedef bool (*protocol_decode_t) (
    const rmt_symbol_word_t *symbols,
    uint8_t num_symbols,
    protocol_command_t *command
);

/**
 * @brief Infrared protocol.
 */
typedef struct protocol {
    const char *name;           /**< Human-readable null-terminated protocol name. */
    uint16_t tolerance_us;      /**< Protocol pulse tolerance in microseconds. */
    uint16_t header_mark_us;    /**< Protocol header pulse high duration in microseconds. */
    uint16_t header_space_us;   /**< Protocol header pulse low duration in microseconds. */
    uint16_t bit0_mark_us;      /**< Protocol logic 0 pulse high duration in microseconds. */
    uint16_t bit1_mark_us;      /**< Protocol logic 1 pulse high duration in microseconds. */
    uint16_t bit_space_us;      /**< Protocol bit space low duration in microseconds. */
    uint16_t footer_mark_us;    /**< Protocol footer pulse high duration in microseconds. */
    uint16_t footer_space_us;   /**< Protocol footer pulse low duration in microseconds. */
    protocol_decode_t decode;   /**< Pointer to protocol decoder function. */
} protocol_t;

/**
 * @brief Return protocol command definition by name.
 *
 * @param[in] command Protocol command name.
 * @param[in] protocol Protocol name.
 *
 * @return Pointer to protocol command structure.
 */
const protocol_command_t *protocol_get_command(
    protocol_command_name_t command, protocol_name_t protocol
);

/**
 * @brief Return protocol definition by name.
 *
 * @param[in] protocol Protocol name.
 *
 * @return Pointer to protocol structure.
 */
const protocol_t *protocol_get_protocol(protocol_name_t protocol);

/**
 * @brief Returns the protocol name.
 *
 * @param[in] protocol Protocol name.
 *
 * @return Human-readable null-terminated string representing the name.
 */
const char *protocol_get_protocol_name(protocol_name_t protocol);

/**
 * @brief Decodes Sony SIRC 12-bits, 15-bits or 20-bits frames.
 */
bool protocol_decode_sony(
    const rmt_symbol_word_t *symbols,
    uint8_t num_symbols,
    protocol_command_t *command
);
