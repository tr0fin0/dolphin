idf_component_register(
    SRCS "encoder.c"
    INCLUDE_DIRS "include"
    REQUIRES
        esp_driver_gpio
        esp_timer
        logging
        pinout
        pwm
        soc
)