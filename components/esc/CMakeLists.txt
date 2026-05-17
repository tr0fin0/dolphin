idf_component_register(
    SRCS "esc.c"
    INCLUDE_DIRS "include"
    REQUIRES driver logging pinout pwm
)