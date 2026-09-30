#include "config.h"
#include "controller.h"
#include "esc.h"
#include "fsm.h"
#include "ir.h"
#include "led.h"
#include "logging.h"
#include "opening.h"
#include "radio.h"

static const controller_sequence_t opening_static_sequence = {
    .commands = NULL,
    .length   = 0,
    .repeat   = false,
};

static const controller_command_t opening_draw_commands[] = {
    {
        .duration_us =  80000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = +90
    },
};
static const controller_sequence_t opening_draw_sequence = {
    .commands = opening_draw_commands,
    .length   = sizeof(opening_draw_commands) / sizeof(opening_draw_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_n_commands[] = {
    {
        .duration_us = 140000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = +90
    },
};
static const controller_sequence_t opening_n_sequence = {
    .commands = opening_n_commands,
    .length   = sizeof(opening_n_commands) / sizeof(opening_n_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_ne_commands[] = {
    {
        .duration_us =  25000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = +90
    },
    {
        .duration_us = 140000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = +90
    },
    {
        .duration_us =  65000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = -90
    },
};
static const controller_sequence_t opening_ne_sequence = {
    .commands = opening_ne_commands,
    .length   = sizeof(opening_ne_commands) / sizeof(opening_ne_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_nw_commands[] = {
    {
        .duration_us =  25000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = -90
    },
    {
        .duration_us = 140000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = +90
    },
    {
        .duration_us =  65000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = +90
    },
};
static const controller_sequence_t opening_nw_sequence = {
    .commands = opening_nw_commands,
    .length   = sizeof(opening_nw_commands) / sizeof(opening_nw_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_s_commands[] = {
    {
        .duration_us = 120000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = -90
    },
};
static const controller_sequence_t opening_s_sequence = {
    .commands = opening_s_commands,
    .length   = sizeof(opening_s_commands) / sizeof(opening_s_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_se_commands[] = {
    {
        .duration_us =  25000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = -90
    },
    {
        .duration_us = 120000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = -90
    },
    {
        .duration_us =  60000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = +90
    },
};
static const controller_sequence_t opening_se_sequence = {
    .commands = opening_se_commands,
    .length   = sizeof(opening_se_commands) / sizeof(opening_se_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_sen_commands[] = {
    {
        .duration_us =  25000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = -90
    },
    {
        .duration_us = 120000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = -90
    },
};
static const controller_sequence_t opening_sen_sequence = {
    .commands = opening_sen_commands,
    .length   = sizeof(opening_sen_commands) / sizeof(opening_sen_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_sw_commands[] = {
    {
        .duration_us =  30000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = +90
    },
    {
        .duration_us = 120000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = -90
    },
    {
        .duration_us =  60000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = -90
    },
};
static const controller_sequence_t opening_sw_sequence = {
    .commands = opening_sw_commands,
    .length   = sizeof(opening_sw_commands) / sizeof(opening_sw_commands[0]),
    .repeat   = false,
};

static const controller_command_t opening_swn_commands[] = {
    {
        .duration_us =  30000,
        .motion = CONTROLLER_MOTION_ROTATION,
        .power = -90
    },
    {
        .duration_us = 120000,
        .motion = CONTROLLER_MOTION_TRANSLATION,
        .power = -90
    },
};
static const controller_sequence_t opening_swn_sequence = {
    .commands = opening_swn_commands,
    .length   = sizeof(opening_sw_commands) / sizeof(opening_swn_commands[0]),
    .repeat   = false,
};

static opening_handler_t opening_handler = {
    .name = "Opening Handler",
    .codes_names = {
        [OPENING_CODE_H] = "CODE_H",
        [OPENING_CODE_L] = "CODE_L",
        [OPENING_CODE_N] = "CODE_N",
    },
    .state          = OPENING_STATE_SELECTION,
    .states_names   = {
        [OPENING_STATE_EXECUTION] = "EXECUTING",
        [OPENING_STATE_FINISHED]  = "FINISHED",
        [OPENING_STATE_RELEASE]   = "RELEASE",
        [OPENING_STATE_SELECTION] = "SELECTING",
    },
    .step = OPENING_STEP_0,
    .strategy   = OPENING_STATIC,
    .strategies = {
        [OPENING_STATIC] = {
            .name = "STATIC",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_N,
                [OPENING_STEP_2] = OPENING_CODE_N,
            },
            .sequence = &opening_static_sequence,
        },
        [OPENING_DRAW]   = {
            .name = "DRAW",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_L,
                [OPENING_STEP_1] = OPENING_CODE_N,
                [OPENING_STEP_2] = OPENING_CODE_N,
            },
            .sequence = &opening_draw_sequence,
        },
        [OPENING_N]      = {
            .name = "NORTH",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_H,
                [OPENING_STEP_2] = OPENING_CODE_N,
            },
            .sequence = &opening_n_sequence,
        },
        [OPENING_NE]     = {
            .name = "NORTH-EAST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_L,
                [OPENING_STEP_1] = OPENING_CODE_H,
                [OPENING_STEP_2] = OPENING_CODE_H,
            },
            .sequence = &opening_ne_sequence,
        },
        [OPENING_NW]     = {
            .name = "NORTH-WEST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_H,
                [OPENING_STEP_1] = OPENING_CODE_H,
                [OPENING_STEP_2] = OPENING_CODE_L,
            },
            .sequence = &opening_nw_sequence,
        },
        [OPENING_S]      = {
            .name = "SOUTH",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_N,
            },
            .sequence = &opening_s_sequence,
        },
        [OPENING_SE]     = {
            .name = "SOUTH-EAST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_H,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_H,
            },
            .sequence = &opening_se_sequence,
        },
        [OPENING_SEN]    = {
            .name = "SOUTH-EAST-NEUTRAL",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_H,
            },
            .sequence = &opening_sen_sequence,
        },
        [OPENING_SW]     = {
            .name = "SOUTH-WEST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_L,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_L,
            },
            .sequence = &opening_sw_sequence,
        },
        [OPENING_SWN]    = {
            .name = "SOUTH-WEST-NEUTRAL",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_L,
            },
            .sequence = &opening_swn_sequence,
        },
    },
    .last_button = OPENING_INITIAL_BUTTON,
};

/**
 * @brief Decodes sequence of @ref opening_code_t into @ref opening_t.
 */
static void opening_decode_strategy(void) {
    for (opening_t strategy = 0; strategy < NUMBER_OF_OPENINGS; strategy++) {
        bool strategy_match = true;
        for (opening_step_t step = 0; step < NUMBER_OF_OPENING_STEPS; step++) {
            if (
                opening_handler.code[step] !=
                opening_handler.strategies[strategy].code[step]
            ) {
                strategy_match = false;
                break;
            }
        }

        if (strategy_match) {
            opening_handler.strategy = strategy;

            LOG_I(
                "opening strategy selected is %s",
                opening_get_strategy_name(opening_handler.strategy)
            );

            break;
        }
    }
}

/**
 * @brief Opening strategy execution.
 */
static void opening_execution(void) {
    const controller_sequence_t *sequence = opening_handler.strategies[
        opening_handler.strategy
    ].sequence;

    if (controller_get_state() == CONTROLLER_STATE_IDLE) {
        if (!controller_start(sequence)) {
            return;
        }
    }

    controller_step();

    if (controller_get_state() == CONTROLLER_STATE_IDLE) {
        opening_handler.state = OPENING_STATE_FINISHED;
    }
}

/**
 * @brief Opening strategy wait release command.
 *
 * The @ref config_control_mode_t defines the release condition:
 *
 * - In @ref CONFIG_CONTROL_AUTONOMOUS wait for @ref IR_STATE_START
 *
 * - In @ref CONFIG_CONTROL_RADIO wait for another button press.
 */
static void opening_release(void) {
    switch (CONFIG_CONTROL_MODE) {
        case CONFIG_CONTROL_AUTONOMOUS:
            if (ir_get_state() == IR_STATE_START) {
                opening_handler.state = OPENING_STATE_EXECUTION;
            }
            break;

        case CONFIG_CONTROL_RADIO:
            pwm_norm_t button = radio_read_channel(RADIO_CHANNEL_3);

            if (opening_handler.last_button != button) {
                opening_handler.state = OPENING_STATE_EXECUTION;
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Opening strategy selection depending on radio receiver model.
 *
 * Available radio receiver models are:
 *
 * - ``FS-GT2``: iterative measures of a single radio receiver channel.
 */
static void opening_selection(void) {
    pwm_norm_t button   = radio_read_channel(RADIO_CHANNEL_3);
    pwm_norm_t throttle = radio_read_channel(RADIO_CHANNEL_2);

    // ensure initial button value is not PWM_NEUTRAL_US
    if (
        (button != PWM_NEUTRAL_US) &&
        (opening_handler.last_button == PWM_NEUTRAL_US)
    ) {
        opening_handler.last_button = button;
    }

    // opening selection via sequential throttle value measures
    if (opening_handler.last_button != button) {
        opening_handler.last_button  = button;
        led_set_toggle(LED_STATE, 100);

        opening_code_t code = OPENING_CODE_N;
        if (throttle > (PWM_NEUTRAL_US+PWM_MAXIMUM_US)/2) code = OPENING_CODE_H;
        if (throttle < (PWM_NEUTRAL_US+PWM_MINIMUM_US)/2) code = OPENING_CODE_L;

        opening_handler.code[opening_handler.step] = code;
        LOG_I("received opening %s", opening_get_code_name(code));

        opening_handler.step++;
    }

    if (opening_handler.step == NUMBER_OF_OPENING_STEPS) {
        opening_decode_strategy();
        opening_handler.state = OPENING_STATE_RELEASE;

        led_set_color(LED_STATE, LED_COLOR_BLUE_LIGHT);
    }
}

void opening_entry(void) {
    controller_stop();

    opening_handler.last_button = radio_read_channel(RADIO_CHANNEL_3);

}

const char *opening_get_code_name(opening_code_t code) {
    if (code >= NUMBER_OF_OPENING_CODES) {
        return NULL;
    }

    return opening_handler.codes_names[code];
}

opening_state_t opening_get_state(void) {
    return opening_handler.state;
}

const char *opening_get_state_name(void) {
    return opening_handler.states_names[opening_handler.state];
}

const char *opening_get_strategy_name(opening_t strategy) {
    if (strategy >= NUMBER_OF_OPENINGS) {
        return NULL;
    }

    return opening_handler.strategies[strategy].name;
}

void opening_run(void) {
    if (radio_get_status() == RADIO_DISCONNECTED) {
        return fsm_transition(STATE_SAFE);
    }

    switch (opening_handler.state) {
        case OPENING_STATE_EXECUTION:
            opening_execution();
            break;

        case OPENING_STATE_FINISHED:
            switch (CONFIG_CONTROL_MODE) {
                case CONFIG_CONTROL_AUTONOMOUS:
                    if (ir_get_state() == IR_STATE_START) {
                        fsm_transition(STATE_AUTONOMOUS);
                    }
                    break;

                case CONFIG_CONTROL_RADIO:
                    if (radio_get_status() == RADIO_CONNECTED) {
                        fsm_transition(STATE_MANUAL);
                    }
                    break;

                default:
                    break;
            }
            break;

        case OPENING_STATE_RELEASE:
            opening_release();
            break;

        case OPENING_STATE_SELECTION:
            opening_selection();
            break;

        default:
            break;
    }
}
