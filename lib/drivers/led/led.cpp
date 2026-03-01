#include <FastLED.h>
#include "colors.h"
#include "debug.h"
#include "led.h"
#include "pinout.h"



static CRGB leds[NUMBER_OF_LEDS];

const uint8_t COLORS[][3] = {
    {COLOR_RED},
    {COLOR_ORANGE_DARK},
    {COLOR_ORANGE_LIGHT},
    {COLOR_YELLOW},
    {COLOR_GREEN_LIME},
    {COLOR_GREEN_LIGHT},
    {COLOR_GREEN},
    {COLOR_EMERALD},
    {COLOR_CIAN},
    {COLOR_BLUE_LIGHT},
    {COLOR_BLUE},
    {COLOR_PURPLE},
    {COLOR_PINK},
    {COLOR_SCARLET},
    {COLOR_WHITE},
    {COLOR_OFF}
};

const size_t COLORS_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);



void led_clear() {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "clearing LEDs configurations");

    FastLED.clear(true);
}


void led_init() {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization of LEDs started");
    led_clear();

    FastLED.addLeds<WS2812B, PIN_RGB_LEDS, GRB>(leds, NUMBER_OF_LEDS);

    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization of LEDs finish");
}


void led_set_brightness(brightness_t brightness) {
    FastLED.setBrightness(brightness);
    FastLED.show();

    DEBUG_MSG(DEBUG_LEVEL_TRACE, "setting LEDs brigthness to %d", brightness);
}


void led_set_color(led_t led, uint8_t r, uint8_t g, uint8_t b) {
    leds[led] = CRGB(r, g, b);
    FastLED.show();

    DEBUG_MSG(
        DEBUG_LEVEL_TRACE, "setting LED %d color to (%d, %d, %d)", led, r, g, b
    );
}


void led_set_color_all(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        leds[i] = CRGB(r, g, b);
    }
    FastLED.show();

    DEBUG_MSG(
        DEBUG_LEVEL_TRACE, "setting all LEDs color to (%d, %d, %d)", r, g, b
    );
}


void led_validate() {
    led_set_brightness(BRIGHTNESS_LOW);

    for (size_t i = 0; i < COLORS_COUNT; i++) {
        led_set_color_all(COLORS[i][0], COLORS[i][1], COLORS[i][2]);
        FastLED.show();

        delay(250);
    }
}
