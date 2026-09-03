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
 * @brief LED brightnesses in percentage.
 */
typedef enum led_brightness {
    LED_BRIGHTNESS_MIN = 0,         /**< LED brightness at ``  0 %``. */
    LED_BRIGHTNESS_MED = 50,        /**< LED brightness at `` 50 %``. */
    LED_BRIGHTNESS_MAX = 100,       /**< LED brightness at ``100 %``. */
    NUMBER_OF_LED_BRIGHTNESSES = 3  /**< Number of LED brightnesses. */
} led_brightness_t;

/**
 * @brief LED colors in RGB.
 */
typedef enum led_color {
    LED_COLOR_BLACK,        /**< ``{.r=  0, .g=  0, .b=  0}`` */
    LED_COLOR_BLUE,         /**< ``{.r=  0, .g=  0, .b=255}`` */
    LED_COLOR_BLUE_LIGHT,   /**< ``{.r=  0, .g= 90, .b=255}`` */
    LED_COLOR_CYAN,         /**< ``{.r=  0, .g=255, .b=255}`` */
    LED_COLOR_EMERALD,      /**< ``{.r=  0, .g=250, .b= 40}`` */
    LED_COLOR_GREEN,        /**< ``{.r=  0, .g=230, .b=  0}`` */
    LED_COLOR_GREEN_LIGHT,  /**< ``{.r=  0, .g=220, .b= 20}`` */
    LED_COLOR_GREEN_LIME,   /**< ``{.r=163, .g=251, .b=  0}`` */
    LED_COLOR_ORANGE_DARK,  /**< ``{.r=254, .g= 23, .b=  0}`` */
    LED_COLOR_ORANGE_LIGHT, /**< ``{.r=255, .g= 48, .b=  0}`` */
    LED_COLOR_PINK,         /**< ``{.r=240, .g=  0, .b= 80}`` */
    LED_COLOR_PURPLE,       /**< ``{.r=252, .g=  3, .b=232}`` */
    LED_COLOR_RED,          /**< ``{.r=255, .g=  0, .b=  0}`` */
    LED_COLOR_SCARLET,      /**< ``{.r=255, .g=  0, .b=  6}`` */
    LED_COLOR_WHITE,        /**< ``{.r=255, .g=255, .b=255}`` */
    LED_COLOR_YELLOW,       /**< ``{.r=255, .g=115, .b=  0}`` */
    NUMBER_OF_LED_COLORS    /**< Number of LED colors. */
} led_color_t;

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
 * @brief LED array configuration.
 */
typedef struct led_array {
    const char *name;                               /**< Human-readable null-terminated LED array name.*/
    pin_t pin;                                      /**< LED array pin connection. */
    led_strip_handle_t strip;                       /**< LED strip handle. */
    led_brightness_t brightness[NUMBER_OF_LEDS];    /**< LEDs brightness in percentage. */
    led_color_t colors[NUMBER_OF_LEDS];             /**< LEDs color in RGB. */
    led_state_t states[NUMBER_OF_LEDS];             /**< LEDs operation  states. */
    int64_t intervals_us[NUMBER_OF_LEDS];           /**< LEDs animation interval in microseconds. */
    int64_t last_time_us[NUMBER_OF_LEDS];           /**< LEDs last update time in microseconds. */
} led_array_t;

/**
 * @def LED_RESOLUTION_HZ
 * @brief LED Strip RMT tick resolution.
 *
 * **Default Value:** 10 MHz
 */
#define LED_RESOLUTION_HZ   10000000

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
