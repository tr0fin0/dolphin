/**
 * @file colors.h
 * @brief RGB LED colors.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#include <stdint.h>

/**
 * @brief LED RGB color.
 */
typedef struct led_color {
    uint8_t r;  /**< Red color channel ranging between 0 and 255. */
    uint8_t g;  /**< Green color channel ranging between 0 and 255. */
    uint8_t b;  /**< Blue color channel ranging between 0 and 255. */
} led_color_t;

/**
 * @def LED_COLOR_BLACK
 * @brief LED color Black.
 *
 * @note When applied, LED turns off.
 *
 * **Default Value:** `{.r= 0, .g= 0, .b= 0}`
 */
#define LED_COLOR_BLACK         (led_color_t) {.r=  0, .g=  0, .b=   0}

/**
 * @def LED_COLOR_BLUE
 * @brief LED color pure Blue.
 *
 * **Default Value:** `{.r= 0, .g= 0, .b= 255}`
 */
#define LED_COLOR_BLUE          (led_color_t) {.r=  0, .g=  0, .b= 255}

/**
 * @def LED_COLOR_BLUE_LIGHT
 * @brief LED color Light Blue.
 *
 * **Default Value:** `{.r= 0, .g= 90, .b= 255}`
 */
#define LED_COLOR_BLUE_LIGHT    (led_color_t) {.r=  0, .g= 90, .b= 255}

/**
 * @def LED_COLOR_CYAN
 * @brief LED color Cyan.
 *
 * **Default Value:** `{.r= 0, .g= 255, .b= 255}`
 */
#define LED_COLOR_CYAN          (led_color_t) {.r=  0, .g=255, .b= 255}

/**
 * @def LED_COLOR_EMERALD
 * @brief LED color Emerald.
 *
 * **Default Value:** `{.r= 0, .g= 250, .b= 40}`
 */
#define LED_COLOR_EMERALD       (led_color_t) {.r=  0, .g=250, .b=  40}

/**
 * @def LED_COLOR_GREEN
 * @brief LED color Pure Green.
 *
 * **Default Value:** `{.r= 0, .g= 230, .b= 0}`
 */
#define LED_COLOR_GREEN         (led_color_t) {.r=  0, .g=230, .b=   0}

/**
 * @def LED_COLOR_GREEN_LIGHT
 * @brief LED color Light Green.
 *
 * **Default Value:** `{.r= 0, .g= 220, .b= 20}`
 */
#define LED_COLOR_GREEN_LIGHT   (led_color_t) {.r=  0, .g=220, .b=  20}

/**
 * @def LED_COLOR_GREEN_LIME
 * @brief LED color Lime Green.
 *
 * **Default Value:** `{.r= 163, .g= 251, .b= 0}`
 */
#define LED_COLOR_GREEN_LIME    (led_color_t) {.r=163, .g=251, .b=   0}

/**
 * @def LED_COLOR_ORANGE_DARK
 * @brief LED color Dark Orange.
 *
 * **Default Value:** `{.r= 254, .g= 23, .b= 0}`
 */
#define LED_COLOR_ORANGE_DARK   (led_color_t) {.r=254, .g= 23, .b=   0}

/**
 * @def LED_COLOR_ORANGE_LIGHT
 * @brief LED color Light Orange.
 *
 * **Default Value:** `{.r= 255, .g= 48, .b= 0}`
 */
#define LED_COLOR_ORANGE_LIGHT  (led_color_t) {.r=255, .g= 48, .b=   0}

/**
 * @def LED_COLOR_PINK
 * @brief LED color Pink.
 *
 * **Default Value:** `{.r= 240, .g= 0, .b= 80}`
 */
#define LED_COLOR_PINK          (led_color_t) {.r=240, .g=  0, .b=  80}

/**
 * @def LED_COLOR_PURPLE
 * @brief LED color Purple.
 *
 * **Default Value:** `{.r= 252, .g= 3, .b= 232}`
 */
#define LED_COLOR_PURPLE        (led_color_t) {.r=252, .g=  3, .b= 232}

/**
 * @def LED_COLOR_RED
 * @brief LED color Pure Red.
 *
 * **Default Value:** `{.r= 255, .g= 0, .b= 0}`
 */
#define LED_COLOR_RED           (led_color_t) {.r=255, .g=  0, .b=   0}

/**
 * @def LED_COLOR_SCARLET
 * @brief LED color Scarlet.
 *
 * **Default Value:** `{.r= 255, .g= 0, .b= 6}`
 */
#define LED_COLOR_SCARLET       (led_color_t) {.r=255, .g=  0, .b=   6}

/**
 * @def LED_COLOR_WHITE
 * @brief LED color Pure White.
 *
 * **Default Value:** `{.r= 255, .g= 255, .b= 255}`
 */
#define LED_COLOR_WHITE         (led_color_t) {.r=255, .g=255, .b= 255}

/**
 * @def LED_COLOR_YELLOW
 * @brief LED color Yellow.
 *
 * **Default Value:** `{.r= 255, .g= 115, .b= 0}`
 */
#define LED_COLOR_YELLOW        (led_color_t) {.r=255, .g=115, .b=   0}
