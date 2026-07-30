idf_component_register(
    SRCS "controller.c"
    INCLUDE_DIRS "include"
    REQUIRES
        config
        esc
        logging
        odometry
        pid
        pwm
)