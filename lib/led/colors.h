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
 * 
 * Available values are:
 * 
 *
 * - `LED_COLOR_BLACK`
 * 
 * - `LED_COLOR_BLUE`
 * 
 * - `LED_COLOR_BLUE_LIGHT`
 * 
 * - `LED_COLOR_CYAN`
 * 
 * - `LED_COLOR_EMERALD`
 * 
 * - `LED_COLOR_GREEN`
 * 
 * - `LED_COLOR_GREEN_LIGHT`
 * 
 * - `LED_COLOR_GREEN_LIME`
 * 
 * - `LED_COLOR_ORANGE_DARK`
 * 
 * - `LED_COLOR_ORANGE_LIGHT`
 * 
 * - `LED_COLOR_PINK`
 * 
 * - `LED_COLOR_PURPLE`
 * 
 * - `LED_COLOR_RED`
 * 
 * - `LED_COLOR_SCARLET`
 * 
 * - `LED_COLOR_WHITE`
 * 
 * - `LED_COLOR_YELLOW`
 * 
 */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color_t;

#define LED_COLOR_BLACK         (led_color_t) {.r=  0, .g=  0, .b=   0}
#define LED_COLOR_BLUE          (led_color_t) {.r=  0, .g=  0, .b= 255}
#define LED_COLOR_BLUE_LIGHT    (led_color_t) {.r=  0, .g= 90, .b= 255}
#define LED_COLOR_CYAN          (led_color_t) {.r=  0, .g=255, .b= 255}
#define LED_COLOR_EMERALD       (led_color_t) {.r=  0, .g=250, .b=  40}
#define LED_COLOR_GREEN         (led_color_t) {.r=  0, .g=230, .b=   0}
#define LED_COLOR_GREEN_LIGHT   (led_color_t) {.r=  0, .g=220, .b=  20}
#define LED_COLOR_GREEN_LIME    (led_color_t) {.r=163, .g=251, .b=   0}
#define LED_COLOR_ORANGE_DARK   (led_color_t) {.r=254, .g= 23, .b=   0}
#define LED_COLOR_ORANGE_LIGHT  (led_color_t) {.r=255, .g= 48, .b=   0}
#define LED_COLOR_PINK          (led_color_t) {.r=240, .g=  0, .b=  80}
#define LED_COLOR_PURPLE        (led_color_t) {.r=252, .g=  3, .b= 232}
#define LED_COLOR_RED           (led_color_t) {.r=255, .g=  0, .b=   0}
#define LED_COLOR_SCARLET       (led_color_t) {.r=255, .g=  0, .b=   6}
#define LED_COLOR_WHITE         (led_color_t) {.r=255, .g=255, .b= 255}
#define LED_COLOR_YELLOW        (led_color_t) {.r=255, .g=115, .b=   0}
