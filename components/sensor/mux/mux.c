#include "driver/gpio.h"
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

const char *mux_get_name(void) {
    return multiplexer.name;
}

void mux_init(void) {
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

    LOG_I(
        "%s common pin ADC%dCH%d initialized.",
        mux_get_name(),
        MUX_ADC_UNIT,
        MUX_ADC_CHANNEL
    );
}

float mux_read_channel(mux_channel_t channel) {
    for (uint8_t i = 0; i < NUMBER_OF_MUX_ADDRESSES; i++) {
        ESP_ERROR_CHECK(
            gpio_set_level(multiplexer.address[i], ((channel >> i) & 1))
        );
    }
    esp_rom_delay_us(1);
    LOG_V("%s reading channel %02d.", mux_get_name(), channel);

    int adc_value;
    ESP_ERROR_CHECK(
        adc_oneshot_read(multiplexer.adc_handle, MUX_ADC_CHANNEL, &adc_value)
    );

    return ((float) (adc_value / MUX_ADC_RESOLUTION));
}
