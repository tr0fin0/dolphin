#include <Arduino.h>
#include <debug.h>
#include <fsm.h>
#include "led.h"



void setup() {
    log_init_async();

    LOG_I("initialization system start");

    fsm_init();
    led_init();

    LOG_I(I, "initialization system finish");
}



void loop() {
    fsm_step();
    led_step();
}
