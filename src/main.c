#include "encoder.h"
#include "fsm.h"
#include "led.h"
#include "logging.h"
#include "freertos/FreeRTOS.h"
#include "odometry.h"

void app_main(void) {
    logging_init_async();

    encoder_init();
    fsm_init();
    led_init();
    odometry_init();

    while (1) {
        odometry_step();
        led_step();
        fsm_step();

        LOG_E(
            "encoder values %.04f | %.04f",
            encoder_get_angle(ENCODER_L),
            encoder_get_angle(ENCODER_R)
        );

        vTaskDelay(10);
    }
}
