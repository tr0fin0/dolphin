#include "fsm.h"
#include "led.h"
#include "logging.h"
#include "freertos/FreeRTOS.h"

void app_main(void) {
    log_init_async();

    fsm_init();
    led_init();

    while (1) {
        fsm_step();
        led_step();

        vTaskDelay(1);
    }
}
