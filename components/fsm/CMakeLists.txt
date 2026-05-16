idf_component_register(
    SRCS 
        "fsm.c"
        "states/src/attack.c"
        "states/src/boot.c"
        "states/src/countdown.c"
        "states/src/manual.c"
        "states/src/opening.c"
        "states/src/safe.c"
        "states/src/search.c"
        "states/src/survive.c"
    INCLUDE_DIRS
        "include"
        "states/include"
    REQUIRES config esc led logging pwm radio
)