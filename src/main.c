#include "freertos/FreeRTOS.h"
#include "fsm.h"
#include "led.h"
#include "logging.h"

void app_main(void) {
    logging_init_async();

    fsm_init();
    led_init();

    while (1) {
        fsm_step();
        led_step();

        vTaskDelay(1);
    }
}
