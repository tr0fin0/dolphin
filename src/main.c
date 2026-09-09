#include "fsm.h"
#include "ir.h"
#include "led.h"
#include "logging.h"
#include "freertos/FreeRTOS.h"

void app_main(void) {
    logging_init_async();

    fsm_init();
    ir_init();
    led_init();

    while (1) {
        fsm_step();
        ir_poll();
        led_step();

        vTaskDelay(1);
    }
}
