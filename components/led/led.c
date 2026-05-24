#include "colors.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "led.h"
#include "logging.h"
#include "pinout.h"

static led_array_t leds = {
    .name = "WS2812B",
    .pin = PIN_RGB_LEDS
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
        ret = led_strip_set_pixel(
            leds.strip,
            i,
            (leds.colors[i].r * led_brightness_rescale(leds.brightness[i])) >> 8,
            (leds.colors[i].g * led_brightness_rescale(leds.brightness[i])) >> 8,
            (leds.colors[i].b * led_brightness_rescale(leds.brightness[i])) >> 8
        );
        if (ret != ESP_OK) {
            LOG_E(
                "LED %d refresh failed with error %s.", i, esp_err_to_name(ret)
            );
        };
    };

    ret = led_strip_refresh(leds.strip);
    if (ret != ESP_OK) {
        LOG_E("LED refresh failed with error %s.", esp_err_to_name(ret));
    };
}

void led_init(void) {
    esp_err_t ret;

    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        leds.brightness[i]      = LED_BRIGHTNESS_MAX;
        leds.colors[i]          = LED_COLOR_WHITE;
        leds.states[i]          = LED_STATE_IDLE;
        leds.intervals_us[i]    = 0;
        leds.last_time_us[i]    = 0;
    };

    led_strip_config_t strip_config = {
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .led_model              = LED_MODEL_WS2812,
        .max_leds               = NUMBER_OF_LEDS,
        .strip_gpio_num         = PIN_RGB_LEDS,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz  = LED_RESOLUTION_HZ,
        .flags.with_dma = false,
    };

    ret = led_strip_new_rmt_device(&strip_config, &rmt_config, &leds.strip);
    if (ret != ESP_OK) {
        LOG_E(
            "LED RMT device creation failed with error %s.",
            esp_err_to_name(ret)
        );
    };

    ret = led_strip_clear(leds.strip);
    if (ret != ESP_OK) {
        LOG_E("LED clear failed with error %s.", esp_err_to_name(ret));
    };

    LOG_I("%d LEDs initialized on pin %d.", NUMBER_OF_LEDS, leds.pin);
}

void led_set_brightness(led_t led, led_brightness_t brightness) {
    leds.brightness[led] = brightness;

    led_refresh();
}

void led_set_brightness_all(led_brightness_t brightness) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        leds.brightness[i] = brightness;
    };

    led_refresh();
}

void led_set_color(led_t led, led_color_t color) {
    leds.colors[led] = color;

    led_refresh();
}

void led_set_color_all(led_color_t color) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        leds.colors[i] = color;
    };

    led_refresh();
}

void led_set_toggle(led_t led, uint32_t interval_ms) {
    led_brightness_t brightness = leds.brightness[led];
    leds.brightness[led] = LED_BRIGHTNESS_MAX - brightness;

    leds.states[led]        = LED_STATE_TOGGLE;
    leds.intervals_us[led]  = interval_ms * 1000;
    leds.last_time_us[led]  = esp_timer_get_time();

    led_refresh();
}

void led_step(void) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        switch (leds.states[i]) {
            case LED_STATE_IDLE:
                break;

            case LED_STATE_TOGGLE:
                int64_t now_us = esp_timer_get_time();

                if (now_us > leds.last_time_us[i] + leds.intervals_us[i]) {
                    led_brightness_t brightness = leds.brightness[i];
                    leds.brightness[i] = LED_BRIGHTNESS_MAX - brightness;

                    leds.states[i]        = LED_STATE_IDLE;
                    leds.intervals_us[i]  = 0;
                    leds.last_time_us[i]  = 0;

                    led_refresh();
                }
                break;

            default:
                break;
        }
    }
}
