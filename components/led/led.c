#include "colors.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "led.h"
#include "logging.h"
#include "pinout.h"

static led_array_t led_array = {
    .name = "WS2812B",
    .pin  = PIN_LRGB,
    .leds = {
        [LED_STATE] = {
            .name = "state LED",
        },
        [LED_EXTRA] = {
            .name = "extra LED",
        },
    }
};

/**
 * @brief Rescales brightness percentage value from 0 to 255.
 *
 * @param brightness Brightness value in percentage.
 *
 * @return Returns rescaled brightness value.
 */
static uint8_t led_brightness_rescale(led_brightness_t brightness) {
    if (brightness > LED_BRIGHTNESS_MAX) {
        brightness = LED_BRIGHTNESS_MAX;
    }

    return (uint8_t) brightness * 255 / 100;
}

/**
 * @brief Refreshes all LEDs on strip.
 */
static void led_refresh(void) {
    esp_err_t ret;

    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        led_config_t led = led_array.leds[i];

        ret = led_strip_set_pixel(
            led_array.strip,
            i,
            (led.color.r * led_brightness_rescale(led.brightness)) >> 8,
            (led.color.g * led_brightness_rescale(led.brightness)) >> 8,
            (led.color.b * led_brightness_rescale(led.brightness)) >> 8
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s refresh failed with error %s.",
                led_get_name(i),
                esp_err_to_name(ret)
            );
        };
    };

    ret = led_strip_refresh(led_array.strip);
    if (ret != ESP_OK) {
        LOG_E("LED refresh failed with error %s.", esp_err_to_name(ret));
    };
}

const char *led_get_array_name() {
    return led_array.name;
}

const char *led_get_name(led_t led) {
    return led_array.leds[led].name;
}

void led_init(void) {
    esp_err_t ret;

    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        led_config_t led = led_array.leds[i];

        led.brightness      = LED_BRIGHTNESS_MAX;
        led.color           = LED_COLOR_WHITE;
        led.state           = LED_STATE_IDLE;
        led.interval_us     = 0;
        led.last_time_us    = 0;
    };

    led_strip_config_t strip_config = {
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .led_model              = LED_MODEL_WS2812,
        .max_leds               = NUMBER_OF_LEDS,
        .strip_gpio_num         = led_array.pin,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz  = LED_RESOLUTION_HZ,
        .flags.with_dma = false,
    };

    ret = led_strip_new_rmt_device(
        &strip_config, &rmt_config, &led_array.strip
    );
    if (ret != ESP_OK) {
        LOG_E(
            "LED RMT device creation failed with error %s.",
            esp_err_to_name(ret)
        );
    };

    ret = led_strip_clear(led_array.strip);
    if (ret != ESP_OK) {
        LOG_E("LED clear failed with error %s.", esp_err_to_name(ret));
    };

    LOG_I("%02d LEDs initialized on pin %d.", NUMBER_OF_LEDS, led_array.pin);
}

void led_set_brightness(led_t led, led_brightness_t brightness) {
    led_array.leds[led].brightness = brightness;

    led_refresh();
}

void led_set_brightness_all(led_brightness_t brightness) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        led_array.leds[i].brightness = brightness;
    };

    led_refresh();
}

void led_set_color(led_t led, led_color_t color) {
    led_array.leds[led].color = color;

    led_refresh();
}

void led_set_color_all(led_color_t color) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        led_array.leds[i].color = color;
    };

    led_refresh();
}

void led_set_toggle(led_t led, uint32_t interval_ms) {
    led_config_t led_config = led_array.leds[led];

    led_brightness_t brightness = led_config.brightness;
    led_config.brightness = LED_BRIGHTNESS_MAX - brightness;

    led_config.state        = LED_STATE_TOGGLE;
    led_config.interval_us  = interval_ms * 1000;
    led_config.last_time_us = esp_timer_get_time();

    led_refresh();
}

void led_step(void) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        led_config_t led = led_array.leds[i];

        switch (led.state) {
            case LED_STATE_IDLE:
                break;

            case LED_STATE_TOGGLE:
                int64_t now_us = esp_timer_get_time();

                if (now_us > led.last_time_us + led.interval_us) {
                    led_brightness_t brightness = led.brightness;
                    led.brightness  = LED_BRIGHTNESS_MAX - brightness;

                    led.state           = LED_STATE_IDLE;
                    led.interval_us     = 0;
                    led.last_time_us    = 0;

                    led_refresh();
                }
                break;

            default:
                break;
        }
    }
}
