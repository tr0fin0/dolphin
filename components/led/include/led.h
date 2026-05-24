/**
 * @file led.h
 * @brief Addressable RGB LEDs controller.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-12
 */

#pragma once

#include <stdint.h>
#include "colors.h"
#include "led_strip.h"
#include "pinout.h"

/**
 * @brief LED brightness in percentage.
 *
 * Where:
 *
 * - minimal brightness with 0%.
 *
 * - maximal brightness with 100%.
 */
typedef uint8_t led_brightness_t;

/**
 * @brief LED operation state.
 *
 * Available values are:
 *
 * - `LED_STATE_IDLE`
 *
 * - `LED_STATE_TOGGLE`
 */
typedef enum led_state {
    LED_STATE_IDLE = 0, /** Default operation state. */
    LED_STATE_TOGGLE,   /** Brightness inverted for an interval in ms. */
} led_state_t;

/**
 * @brief LED name.
 *
 * Available values are:
 *
 * - `LED_STATE`
 *
 * - `LED_EXTRA`
 */
typedef enum led {
    LED_STATE = 0,  /** LED used for the system state. */
    LED_EXTRA,      /** LED used for debugging. */
    NUMBER_OF_LEDS
} led_t;

/**
 * @brief LED strip configuration.
 */
typedef struct led_array {
    const char *name;
    pin_t pin;
    led_strip_handle_t strip;
    led_brightness_t brightness[NUMBER_OF_LEDS];
    led_color_t colors[NUMBER_OF_LEDS];
    led_state_t states[NUMBER_OF_LEDS];
    int64_t intervals_us[NUMBER_OF_LEDS];
    int64_t last_time_us[NUMBER_OF_LEDS];
} led_array_t;

#define LED_BRIGHTNESS_MIN  0
#define LED_BRIGHTNESS_MED  50
#define LED_BRIGHTNESS_MAX  100
#define LED_RESOLUTION_HZ   10000000

/**
 * @brief Initialization of all individual LEDs on the strip.
 */
void led_init(void);

/**
 * @brief Set the brightness of a LED on the strip.
 *
 * @param led LED name.
 * @param brightness Brightness in percentage.
 */
void led_set_brightness(led_t led, led_brightness_t brightness);

/**
 * @brief Set the brightness of all LEDs on the strip.
 *
 * @param brightness Brightness in percentage.
 */
void led_set_brightness_all(led_brightness_t brightness);

/**
 * @brief Set the RGB color of a LED on the strip.
 *
 * @param led LED name.
 * @param color RGB color.
 */
void led_set_color(led_t led, led_color_t color);

/**
 * @brief Set the RGB color of all LEDs on the strip.
 *
 * @param color RGB color.
 */
void led_set_color_all(led_color_t color);

/**
 * @brief Set a brightness toggle of a LED on the strip.
 *
 * Brightness toggle is an inversion with respect to the medium value.
 *
 * @param led LED name.
 * @param interval_ms Toggle interval in miliseconds.
 */
void led_set_toggle(led_t led, uint32_t interval_ms);

/**
 * @brief Update individual LED values with respect to their states.
 */
void led_step(void);
