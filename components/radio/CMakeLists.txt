idf_component_register(
    SRCS "radio.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        driver
        esp_timer
        logging
        pinout
        pwm
        soc
)