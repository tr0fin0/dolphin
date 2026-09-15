#include "driver/rmt_types.h"
#include "driver/rmt_rx.h"
#include "esp_err.h"
#include "ir.h"
#include "logging.h"
#include "pinout.h"
#include "protocol.h"

/**
 * @brief Updates infrared state based on to the received command.
 *
 * @param[in] command_received Pointer to a protocol command.
 */
static void ir_received(const protocol_command_t *command_received) {
    for (uint8_t i = 0; i < NUMBER_OF_PROTOCOL_COMMANDS; i++) {
        const protocol_command_t *command_evaluated = protocol_get_command(
            (protocol_command_name_t) i, command_received->protocol
        );

        if (
            (command_evaluated->address == command_received->address) &&
            (command_evaluated->command == command_received->command)
        ) {
            ir_state_t current_state = ir_get_state();
            ir_state_t desired_state = (ir_state_t) i;

            switch (current_state) {
                case IR_STATE_READY:
                    if (desired_state == IR_STATE_START) {
                        ir_set_state(desired_state);
                    }
                    break;

                case IR_STATE_START:
                    if (desired_state == IR_STATE_STOP) {
                        ir_set_state(desired_state);
                    }
                    break;

                case IR_STATE_STOP:
                    break;

                case IR_STATE_WAIT:
                    if (desired_state == IR_STATE_READY) {
                        ir_set_state(desired_state);
                    }
                    break;

                default:
                    break;
            }
            break;
        };
    };
};

/**
 * @brief GPIO Interrupt Service Routine for Infrared RMT pulse capture.
 *
 * ISR attached to the RMT on received done callback. If the defined protocol
 * decodes the received event data the reconstructed command is then passed to
 * the @ref ir_received function for treatment.
 *
 * @param[in] rx_channel RMT receive configured channel.
 * @param[in] event_data Pointer to the contents of the triggered event.
 * @param[in] user_data Pointer to the @ref ir_t device defined on the file.
 */
static bool IRAM_ATTR ir_rmt_rx_callback(
    rmt_channel_handle_t rx_channel,
    const rmt_rx_done_event_data_t *event_data,
    void *user_data
) {
    ir_t *device = (ir_t *) user_data;

    if (device->protocol && device->protocol->decode) {
        device->valid_data = device->protocol->decode(
            event_data->received_symbols,
            event_data->num_symbols,
            (protocol_command_t *) &device->decoded_command
        );
    }

    device->rx_done = true;

    return false;
}

static ir_t ir = {
    .name       = "TL1838 IR",
    .callback   = ir_received,
    .active_low = true,
    .pin        = PIN_IRR,
    .state      = IR_STATE_WAIT,
    .state_names = {
        [IR_STATE_READY] = "READY",
        [IR_STATE_START] = "START",
        [IR_STATE_STOP]  = "STOP",
        [IR_STATE_WAIT]  = "WAIT",
    }
};

const char *ir_get_name(void) {
    return ir.name;
};

ir_state_t ir_get_state(void) {
    return ir.state;
};

const char *ir_get_state_name(void) {
    return ir.state_names[ir.state];
};

void ir_init(void) {
    ir.protocol = protocol_get_protocol(PROTOCOL_SONY);

    rmt_rx_channel_config_t rmt_rx_config = {
        .gpio_num           = ir.pin,
        .clk_src            = RMT_CLK_SRC_DEFAULT,
        .resolution_hz      = IR_RMT_CLOCK_RESOLUTION_HZ,
        .mem_block_symbols  = IR_RMT_MEMORY_BLOCK_SYMBOL,
        .flags.invert_in    = ir.active_low,
        .flags.with_dma     = false,
    };
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rmt_rx_config, &ir.rx_channel));

    rmt_rx_event_callbacks_t rmt_rx_callback = {
        .on_recv_done = ir_rmt_rx_callback
    };
    ESP_ERROR_CHECK(
        rmt_rx_register_event_callbacks(ir.rx_channel, &rmt_rx_callback, &ir)
    );

    ESP_ERROR_CHECK(rmt_enable(ir.rx_channel));

    rmt_receive_config_t rmt_receive_config = {
        .signal_range_max_ns = IR_RMT_SIGNAL_MAX_NS,
        .signal_range_min_ns = IR_RMT_SIGNAL_MIN_NS,
    };
    ESP_ERROR_CHECK(
        rmt_receive(
            ir.rx_channel,
            ir.rx_buffer,
            sizeof(ir.rx_buffer),
            &rmt_receive_config
        )
    );

    ir.state = IR_STATE_WAIT;
    LOG_I("%s initialized to state %s", ir_get_name(), ir_get_state_name());
};

void ir_poll(void) {
    if (ir.rx_done) {
        ir.rx_done = false;

        if (ir.valid_data) {
            ir.valid_data = false;
            if (ir.callback) {
                ir.callback((const protocol_command_t *) &ir.decoded_command);
            }
        }

        // Re-arm RMT receiver for next pulse frame
        rmt_receive_config_t rmt_receive_config = {
            .signal_range_max_ns = IR_RMT_SIGNAL_MAX_NS,
            .signal_range_min_ns = IR_RMT_SIGNAL_MIN_NS,
        };
        rmt_receive(
            ir.rx_channel,
            ir.rx_buffer,
            sizeof(ir.rx_buffer),
            &rmt_receive_config
        );
    }
}

void ir_set_state(ir_state_t state) {
    ir.state = state;
};
