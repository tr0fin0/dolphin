idf_component_register(
    SRCS
        "led.c"
        # "led_strip/src/led_strip_api.c"
        # "led_strip/src/led_strip_rmt_dev.c"
        # "led_strip/src/led_strip_rmt_encoder.c"
        # "led_strip/src/led_strip_spi_dev.c"
    INCLUDE_DIRS
        "include"
        # "led_strip/include"
        # "led_strip/interface"
    REQUIRES
        # esp_driver_rmt
        # esp_driver_spi
        esp_timer
        led_strip
        logging
        pinout
)