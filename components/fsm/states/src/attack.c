#include "attack.h"
#include "controller.h"
#include "fsm.h"
#include "ir.h"
#include "sensor.h"

static const controller_command_t attack_commands[] = {
    {
        .duration_us = 50000,
        .motion      = CONTROLLER_MOTION_TRANSLATION,
        .power       = +100
    },
};

static const controller_sequence_t attack_sequence = {
    .commands = attack_commands,
    .length   = sizeof(attack_commands) / sizeof(attack_commands[0]),
    .repeat   = true,
};

void attack_entry(void) {
    controller_stop();
}

void attack_exit(void) {
    controller_stop();
}

void attack_run(void) {
    if (ir_get_state() != IR_STATE_START) {
        return fsm_transition(FSM_STATE_SAFE);
    }

    if (sensor_detected_line()) {
        return fsm_transition(FSM_STATE_SURVIVE);
    }

    if (!sensor_detected_obstacle_front()) {
        return fsm_transition(FSM_STATE_AUTONOMOUS);
    }

    if (controller_get_state() == CONTROLLER_STATE_IDLE) {
        if (!controller_start(&attack_sequence)) {
            return;
        }
    }

    controller_step();
}
