idf_component_register(
    SRCS "esc.c"
    INCLUDE_DIRS "include"
    REQUIRES
        esp_driver_mcpwm
        logging
        pinout
        pwm
)