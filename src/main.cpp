#include <Arduino.h>
#include <debug.h>
#include <fsm.h>



void setup() {
    debug_init(115200);

    LOG_I("initialization system start");

    fsm_init();

    LOG_I(I, "initialization system finish");
}



void loop() {
    fsm_step();
}
