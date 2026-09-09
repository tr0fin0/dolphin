#include "protocol.h"

#define PROTOCOL_SONY_HEADER_MARK_US    2400
#define PROTOCOL_SONY_HEADER_SPACE_US    600
#define PROTOCOL_SONY_BIT0_MARK_US       600
#define PROTOCOL_SONY_BIT1_MARK_US      1200
#define PROTOCOL_SONY_BIT_SPACE_US       600
#define PROTOCOL_SONY_TOLERANCE_US       200

/**
 * @brief Evaluates if the measured value is target within given tolerances.
 *
 * @param[in] value Measured value in us.
 * @param[in] target Target measure in us.
 * @param[in] tolerance Measurement tolerance in us.
 *
 * @return true if value reached target within tolerances, false otherwise.
 */
static bool in_range(uint32_t value, uint32_t target, uint32_t tolerance) {
    return (value > (target - tolerance)) && (value < (target + tolerance));
};

bool protocol_decode_sony(
    const rmt_symbol_word_t *symbols,
    uint8_t num_symbols,
    protocol_command_t *command
) {
    if (!symbols || num_symbols < 13 || !command) {
        return false;
    }

    // header validation
    if (
        !in_range(
            symbols[0].duration0,
            PROTOCOL_SONY_HEADER_MARK_US,
            PROTOCOL_SONY_TOLERANCE_US
        )
        ||
        !in_range(
            symbols[0].duration1,
            PROTOCOL_SONY_HEADER_SPACE_US,
            PROTOCOL_SONY_TOLERANCE_US
        )
    ) {
        return false;
    }

    // data decoding
    uint8_t bits = 0;
    uint32_t data = 0;
    for (uint8_t i = 1; i < num_symbols; i++) {
        uint32_t mark = symbols[i].duration0;

        if (
            in_range(
                mark,
                PROTOCOL_SONY_BIT1_MARK_US,
                PROTOCOL_SONY_TOLERANCE_US
            )
        ) {
            data |= (1UL << bits);
            bits++;
        } else if (
            in_range(
                mark,
                PROTOCOL_SONY_BIT0_MARK_US,
                PROTOCOL_SONY_TOLERANCE_US
            )
        ) {
            bits++;
        } else {
            break;
        }
    }

    // data validation
    if (bits < 12) {
        return false;
    }

    // command reconstruction
    command->bit_count = bits;
    command->address = (data >> 7) & 0x1F;
    command->command = data & 0x7F;
    command->protocol = PROTOCOL_SONY;

    return true;
}

/**
 * @brief All possible commands for each protocol.
 */
static const protocol_command_t
    commands[NUMBER_OF_PROTOCOL_NAMES][NUMBER_OF_PROTOCOL_COMMANDS] = {
    [PROTOCOL_SONY] = {
        [PROTOCOL_COMMAND_READY] = { .address = 0x01, .command = 0x00 },
        [PROTOCOL_COMMAND_START] = { .address = 0x01, .command = 0x01 },
        [PROTOCOL_COMMAND_STOP]  = { .address = 0x01, .command = 0x02 },
    },
};

/**
 * @brief All possible protocol configurations.
 */
static const protocol_t protocols[NUMBER_OF_PROTOCOL_NAMES] = {
    [PROTOCOL_SONY] = {
        .name               = "SONY",
        .header_mark_us     = PROTOCOL_SONY_HEADER_MARK_US,
        .header_space_us    = PROTOCOL_SONY_HEADER_SPACE_US,
        .bit0_mark_us       = PROTOCOL_SONY_BIT0_MARK_US,
        .bit1_mark_us       = PROTOCOL_SONY_BIT1_MARK_US,
        .bit_space_us       = PROTOCOL_SONY_BIT_SPACE_US,
        .tolerance_us       = PROTOCOL_SONY_TOLERANCE_US,
        .decode             = protocol_decode_sony
    },
};

const protocol_command_t *protocol_get_command(
    protocol_command_name_t command, protocol_name_t protocol
) {
    return &commands[protocol][command];
}

const protocol_t *protocol_get_protocol(protocol_name_t protocol) {
    return &protocols[protocol];
}

const char *protocol_get_protocol_name(protocol_name_t protocol) {
    return protocols[protocol].name;
}
