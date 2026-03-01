#include <Arduino.h>
#include <debug.h>
#include <fsm.h>



void setup() {
    debug_init(115200);
    delay(2500);

    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization system start");

    fsm_init();

    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization system finish");
}



void loop() {
    fsm_step();

    // delay(100);
}
