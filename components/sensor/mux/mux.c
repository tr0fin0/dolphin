#include "driver/gpio.h"
#include "ema.h"
#include "esp_err.h"
#include "logging.h"
#include "mux.h"
#include "pinout.h"
#include <stdint.h>

static mux_t multiplexer = {
    .name = "CD74HC4067M",
    .adc_handle = NULL,
    .address = {
        PIN_MUX_ADDR0,
        PIN_MUX_ADDR1,
        PIN_MUX_ADDR2,
        PIN_MUX_ADDR3
    },
    .common = PIN_MUX_COMMON,
#if defined(CONFIG_MAINBOARD_V1)
    .enable = PIN_MUX_ENABLE,
#endif
};

/**
 * @brief Multiplexer Timer Callback
 *
 * Every @ref MUX_TIMER_PERIOD_US this function is called, storing the ADC
 * measure in the respective @ref mux_buffer_t channel.
 */
static void mux_timer_callback(void* arg) {
    mux_channel_t channel = multiplexer.current_channel;
    mux_buffer_t *buffer  = &multiplexer.buffers[channel];

    if (buffer->active) {
        int adc_value = 0;
        ESP_ERROR_CHECK(
            adc_oneshot_read(
                multiplexer.adc_handle, MUX_ADC_CHANNEL, &adc_value
            )
        );

        buffer->measures[buffer->head] = (uint16_t) adc_value;
        buffer->head                   = (buffer->head + 1) % MUX_BUFFER_SIZE;
    }

    do {
        channel = (channel + 1) % NUMBER_OF_MUX_CHANNELS;
    } while (!multiplexer.buffers[channel].active);

    multiplexer.current_channel = channel;

    for (uint8_t i = 0; i < NUMBER_OF_MUX_ADDRESSES; i++) {
        gpio_set_level(
            multiplexer.address[i], ((multiplexer.current_channel >> i) & 0x1)
        );
    }
}

const char *mux_get_name(void) {
    return multiplexer.name;
}

void mux_init(mux_buffer_t *buffers_config) {
#if defined(CONFIG_MAINBOARD_V1)
    gpio_config_t pin_config = {
        .pin_bit_mask   = (1ULL << multiplexer.enable),
        .mode           = GPIO_MODE_OUTPUT,
        .pull_up_en     = GPIO_PULLUP_DISABLE,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .intr_type      = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&pin_config));

    ESP_ERROR_CHECK(gpio_set_level(multiplexer.enable, (uint32_t) 0));
#endif

    for (uint8_t i = 0; i < NUMBER_OF_MUX_ADDRESSES; i++) {
        gpio_config_t pin_config = {
            .pin_bit_mask   = (1ULL << multiplexer.address[i]),
            .mode           = GPIO_MODE_OUTPUT,
            .pull_up_en     = GPIO_PULLUP_DISABLE,
            .pull_down_en   = GPIO_PULLDOWN_DISABLE,
            .intr_type      = GPIO_INTR_DISABLE
        };
        ESP_ERROR_CHECK(gpio_config(&pin_config));
    }
    LOG_I("%s address pins initialized.", mux_get_name());

    uint8_t active_buffers = 0;
    for(uint8_t channel = 0; channel < NUMBER_OF_MUX_CHANNELS; channel++) {
        multiplexer.buffers[channel].active = buffers_config[channel].active;
        if (multiplexer.buffers[channel].active) {
            active_buffers++;
        }

        multiplexer.buffers[channel].head = 0;

        for(uint8_t i = 0; i < MUX_BUFFER_SIZE; i++) {
            multiplexer.buffers[channel].measures[i] = 0;
        }
    }
    multiplexer.current_channel = 0;

    adc_oneshot_unit_init_cfg_t adc_unit_config = {
        .unit_id = MUX_ADC_UNIT,
    };
    ESP_ERROR_CHECK(
        adc_oneshot_new_unit(&adc_unit_config, &multiplexer.adc_handle)
    );

    adc_oneshot_chan_cfg_t adc_channel_config = {
        .atten    = MUX_ADC_ATTENUATION,
        .bitwidth = MUX_ADC_BITWIDTH,
    };
    ESP_ERROR_CHECK(
        adc_oneshot_config_channel(
            multiplexer.adc_handle, MUX_ADC_CHANNEL, &adc_channel_config
        )
    );

    if (active_buffers > 0) {
        const esp_timer_create_args_t timer_args = {
            .name            = "MUX ADC Timer",
            .callback        = &mux_timer_callback,
            .dispatch_method = ESP_TIMER_TASK
        };
        ESP_ERROR_CHECK(
            esp_timer_create(&timer_args, &multiplexer.timer_handle)
        );
        ESP_ERROR_CHECK(
            esp_timer_start_periodic(
                multiplexer.timer_handle, MUX_TIMER_PERIOD_US
            )
        );
    }

    LOG_I(
        "%s common pin ADC%dCH%d initialized.",
        mux_get_name(),
        MUX_ADC_UNIT,
        MUX_ADC_CHANNEL
    );
}

float mux_read_channel(mux_channel_t channel) {
    uint16_t measure = ema_compute_circular(
        multiplexer.buffers[channel].measures,
        MUX_BUFFER_SIZE,
        multiplexer.buffers[channel].head,
        0.3333f
    );

    return ((float) measure / MUX_ADC_RESOLUTION);
}
