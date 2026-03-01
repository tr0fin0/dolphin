#ifndef __LED_H__
#define __LED_H__

#include <stdio.h>
#include "colors.h"


#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Discrete LED brightness levels.
 *
 * Defines predefined intensity values between 0 and 255 used with
 * `FastLED.setBrightness()`.
 */
typedef enum {
    BRIGHTNESS_OFF      =   0,
    BRIGHTNESS_LOW      =  64,
    BRIGHTNESS_MEDIUM   = 128,
    BRIGHTNESS_HIGH     = 192,
    BRIGHTNESS_MAX      = 255,
} brightness_t;

/**
 * @brief Logical LED indices.
 *
 * Defines the ordered mapping of physical LEDs in the system.
 *
 * - `LED_STATE`:   system state indicator.
 *
 * - `LED_EXTRA`:   auxiliary indicator.
 */
typedef enum {
    LED_STATE,      /**< system state indicator. */
    LED_EXTRA,      /**< auxiliary indicator. */
    NUMBER_OF_LEDS
} led_t;



/**
 * @brief Initialize hardware driver for LED.
 *
 * After initialization, LEDs start at `BRIGHTNESS_OFF`.
 */
void led_init(void);

/**
 * @brief Clear internal LED buffer and update strip.
 *
 * `FastLED.clear(true)` is used; this will call `show()` internally.
 */
void led_clear(void);

/**
 * @brief Set global brightness (0..255) and update strip.
 *
 * This will call `FastLED.show()` to apply immediately.
 *
 * @note To reduce refreshes, use `led_set_brightness()` once and
 * perform many `led_set_color_*()` calls.
 */
void led_set_brightness(brightness_t brightness);

/**
 * @brief Set color of all LEDs and update strip.
 *
 * This performs a bulk fill and issues a single `FastLED.show()`.
 */
void led_set_color_all(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Set color of a single LED and update strip.
 */
void led_set_color(led_t led, uint8_t r, uint8_t g, uint8_t b);


void led_validate(void);



#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */
