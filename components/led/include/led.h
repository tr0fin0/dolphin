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
 * - **minimal** brightness with `0%`.
 *
 * - **maximal** brightness with `100%`.
 */
typedef uint8_t led_brightness_t;

/**
 * @brief LED operation state.
 */
typedef enum led_state {
    LED_STATE_IDLE = 0,     /**< Default operation state. */
    LED_STATE_TOGGLE,       /**< Brightness inverted for an interval in ms. */
    NUMBER_OF_LED_STATES    /**< Number of LED states. */
} led_state_t;

/**
 * @brief LED names.
 */
typedef enum led {
    LED_STATE = 0,  /**< LED used for indicating the system state. */
    LED_EXTRA,      /**< LED used for debugging. */
    NUMBER_OF_LEDS  /**< Number of distinct LEDs. */
} led_t;

/**
 * @brief LED configuration.
 */
typedef struct led_config {
    const char *name;               /**< Human-readable null-terminated LED name.*/
    led_brightness_t brightness;    /**< LED brightness in percentage. */
    led_color_t color;              /**< LED RGB color. */
    led_state_t state;              /**< LED operation state. */
    int64_t interval_us;            /**< LED animation interval in microseconds. */
    int64_t last_time_us;           /**< LED last update time in microseconds. */
} led_config_t;

/**
 * @brief LED array configuration.
 */
typedef struct led_array {
    const char *name;                   /**< Human-readable null-terminated LED array name.*/
    pin_t pin;                          /**< LED array pin connection. */
    led_strip_handle_t strip;           /**< LED strip handle. */
    led_config_t leds[NUMBER_OF_LEDS];  /**< LEDs configurations. */
} led_array_t;

/**
 * @def LED_BRIGHTNESS_MIN
 * @brief LED minimal brightness percentage.
 *
 * **Default Value:** 0%
 */
#define LED_BRIGHTNESS_MIN  0

/**
 * @def LED_BRIGHTNESS_MED
 * @brief LED medium brightness percentage.
 *
 * **Default Value:** 50%
 */
#define LED_BRIGHTNESS_MED  50

/**
 * @def LED_BRIGHTNESS_MAX
 * @brief LED maximal brightness percentage.
 *
 * **Default Value:** 100%
 */
#define LED_BRIGHTNESS_MAX  100

/**
 * @def LED_RESOLUTION_HZ
 * @brief LED Strip RMT tick resolution.
 *
 * **Default Value:** 10 MHz
 */
#define LED_RESOLUTION_HZ   10000000

/**
 * @brief Returns the LED array name.
 *
 * @return Human-readable null-terminated string representing the name.
 */
const char *led_get_array_name(void);

/**
 * @brief Returns the LED name.
 *
 * @param[in] led LED name.
 * @return Human-readable null-terminated string representing the name.
 */
const char *led_get_name(led_t led);

/**
 * @brief Initialization of all individual LEDs on the strip.
 */
void led_init(void);

/**
 * @brief Set the brightness of a LED on the strip.
 *
 * @param[in] led LED identifier.
 * @param[in] brightness Brightness in percentage.
 */
void led_set_brightness(led_t led, led_brightness_t brightness);

/**
 * @brief Set the brightness of all LEDs on the strip.
 *
 * @param[in] brightness Brightness in percentage.
 */
void led_set_brightness_all(led_brightness_t brightness);

/**
 * @brief Set the RGB color of a LED on the strip.
 *
 * @param[in] led LED identifier.
 * @param[in] color RGB color.
 */
void led_set_color(led_t led, led_color_t color);

/**
 * @brief Set the RGB color of all LEDs on the strip.
 *
 * @param[in] color RGB color.
 */
void led_set_color_all(led_color_t color);

/**
 * @brief Set a brightness toggle of a LED on the strip.
 *
 * Brightness toggle is an inversion with respect to the @ref LED_BRIGHTNESS_MED
 * value.
 *
 * @param[in] led LED identifier.
 * @param[in] interval_ms Toggle interval in milliseconds.
 */
void led_set_toggle(led_t led, uint32_t interval_ms);

/**
 * @brief Update individual LED values with respect to their states.
 */
void led_step(void);
