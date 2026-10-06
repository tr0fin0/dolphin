#include "controller.h"
#include "fsm.h"
#include "ir.h"
#include "logging.h"
#include "sensor.h"
#include "survive.h"

static const controller_sequence_t survive_sequence_static = {
    .commands = NULL,
    .length   = 0,
    .repeat   = false,
};

static const controller_command_t survive_commands_b[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_TRANSLATION,
        .power       = +100
    },
};
static const controller_sequence_t survive_sequence_b = {
    .commands = survive_commands_b,
    .length   = sizeof(survive_commands_b) / sizeof(survive_commands_b[0]),
    .repeat   = false
};

static const controller_command_t survive_commands_bl[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_TRANSLATION,
        .power       = +100
    },
};
static const controller_sequence_t survive_sequence_bl = {
    .commands = survive_commands_bl,
    .length   = sizeof(survive_commands_bl) / sizeof(survive_commands_bl[0]),
    .repeat   = false
};

static const controller_command_t survive_commands_br[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_TRANSLATION,
        .power       = +100
    },
};
static const controller_sequence_t survive_sequence_br = {
    .commands = survive_commands_br,
    .length   = sizeof(survive_commands_br) / sizeof(survive_commands_br[0]),
    .repeat   = false
};

static const controller_command_t survive_commands_f[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_TRANSLATION,
        .power       = -100
    },
};
static const controller_sequence_t survive_sequence_f = {
    .commands = survive_commands_f,
    .length   = sizeof(survive_commands_f) / sizeof(survive_commands_f[0]),
    .repeat   = false
};

static const controller_command_t survive_commands_fl[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_TRANSLATION,
        .power       = -100
    },
};
static const controller_sequence_t survive_sequence_fl = {
    .commands = survive_commands_fl,
    .length   = sizeof(survive_commands_fl) / sizeof(survive_commands_fl[0]),
    .repeat   = false
};

static const controller_command_t survive_commands_fr[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_TRANSLATION,
        .power       = -100
    },
};
static const controller_sequence_t survive_sequence_fr = {
    .commands = survive_commands_fr,
    .length   = sizeof(survive_commands_fr) / sizeof(survive_commands_fr[0]),
    .repeat   = false
};

static const controller_command_t survive_commands_l[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_ROTATION,
        .power       = +100
    },
};
static const controller_sequence_t survive_sequence_l = {
    .commands = survive_commands_l,
    .length   = sizeof(survive_commands_l) / sizeof(survive_commands_l[0]),
    .repeat   = false
};

static const controller_command_t survive_commands_r[] = {
    {
        .duration_us = 1000,
        .motion      = CONTROLLER_MOTION_ROTATION,
        .power       = -100
    },
};
static const controller_sequence_t survive_sequence_r = {
    .commands = survive_commands_r,
    .length   = sizeof(survive_commands_r) / sizeof(survive_commands_r[0]),
    .repeat   = false
};

static survive_handler_t survive = {
    .name = "STATE_SURVIVE",
    .move = SURVIVE_MOVE_STATIC,
    .moves = {
        [SURVIVE_MOVE_STATIC]  = {
            .name = "MOVE_STATIC",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MAX_VALUE,
            },
            .sequence = &survive_sequence_static,
        },
        [SURVIVE_MOVE_B]  = {
            .name = "MOVE_B",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MIN_VALUE,
            },
            .sequence = &survive_sequence_b,
        },
        [SURVIVE_MOVE_BL] = {
            .name = "MOVE_BL",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MIN_VALUE,
            },
            .sequence = &survive_sequence_bl,
        },
        [SURVIVE_MOVE_BR] = {
            .name = "MOVE_BR",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MIN_VALUE,
            },
            .sequence = &survive_sequence_br,
        },
        [SURVIVE_MOVE_F]  = {
            .name = "MOVE_F",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MAX_VALUE,
            },
            .sequence = &survive_sequence_f,
        },
        [SURVIVE_MOVE_FL] = {
            .name = "MOVE_FL",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MIN_VALUE,
            },
            .sequence = &survive_sequence_fl,
        },
        [SURVIVE_MOVE_FR] = {
            .name = "MOVE_FL",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MAX_VALUE,
            },
            .sequence = &survive_sequence_fr,
        },
        [SURVIVE_MOVE_L]  = {
            .name = "MOVE_L",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MIN_VALUE,
            },
            .sequence = &survive_sequence_l,
        },
        [SURVIVE_MOVE_R]  = {
            .name = "MOVE_R",
            .sensors_states = {
                [SENSOR_QRE_BL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_BR] = SENSOR_MAX_VALUE,
                [SENSOR_QRE_FL] = SENSOR_MIN_VALUE,
                [SENSOR_QRE_FR] = SENSOR_MAX_VALUE,
            },
            .sequence = &survive_sequence_r,
        },
    },
    .sensors_active = {
        [SENSOR_JS2_DL] = false,
        [SENSOR_JS2_DR] = false,
        [SENSOR_JS2_FL] = false,
        [SENSOR_JS2_FR] = false,
        [SENSOR_JS2_LL] = false,
        [SENSOR_JS2_LR] = false,
        [SENSOR_QRE_BL] = true,
        [SENSOR_QRE_BR] = true,
        [SENSOR_QRE_FL] = true,
        [SENSOR_QRE_FR] = true,
    },
};

static void survive_decode_move(void) {
    for (sensor_t sensor = 0; sensor < NUMBER_OF_SENSORS; sensor++) {
        if (survive.sensors_active[sensor]) {
            survive.sensors_values[sensor] = sensor_get_value(sensor);
        }
    }

    for (survive_move_t move = 0; move < NUMBER_OF_SURVIVE_MOVES; move++) {
        bool move_match = true;
        for (sensor_t sensor = 0; sensor < NUMBER_OF_SENSORS; sensor++) {
            if (!survive.sensors_active[sensor]) {
                continue;
            }

            if (
                survive.sensors_values[sensor] !=
                survive.moves[move].sensors_states[sensor]
            ) {
                move_match = false;
                break;
            }
        }

        if (move_match) {
            survive.move = move;

            LOG_I(
                "survive move is %s",
                survive_get_move_name(survive.move)
            );

            return;
        }
    }

    LOG_E("unable to decode survive move");
}

void survive_entry(void) {
    controller_stop();
}

void survive_exit(void) {
    controller_stop();
}

const char *survive_get_move_name(survive_move_t move) {
    if (move >= NUMBER_OF_SURVIVE_MOVES) {
        return NULL;
    }

    return survive.moves[move].name;
}

void survive_run(void) {
    if (ir_get_state() != IR_STATE_START) {
        return fsm_transition(FSM_STATE_SAFE);
    }

    if (!sensor_detected_line()) {
        return fsm_transition(FSM_STATE_AUTONOMOUS);
    }

    if (controller_get_state() == CONTROLLER_STATE_IDLE) {
        survive_decode_move();

        const controller_sequence_t *sequence = survive.moves[
            survive.move
        ].sequence;

        if (!controller_start(sequence)) {
            return;
        }
    }

    controller_step();
}
