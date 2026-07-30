#include "fsm.h"
#include "led.h"
#include "logging.h"
#include "freertos/FreeRTOS.h"
#include "odometry.h"

void app_main(void) {
    logging_init_async();

    odometry_init();
    led_init();
    fsm_init();

    while (1) {
        odometry_step();
        led_step();
        fsm_step();

        vTaskDelay(1);
    }
}
