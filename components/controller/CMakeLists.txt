idf_component_register(
    SRCS "controller.c"
    INCLUDE_DIRS "include"
    REQUIRES
        config
        esc
        esp_timer
        logging
        odometry
        pid
        pwm
)