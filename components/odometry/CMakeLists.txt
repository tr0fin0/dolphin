idf_component_register(
    SRCS "odometry.c"
    INCLUDE_DIRS "include"
    REQUIRES
        config
        encoder
        esp_timer
        logging
)