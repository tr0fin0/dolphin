#include "controller.h"
#include "fsm.h"
#include "ir.h"
#include "search.h"
#include "sensor.h"

static const controller_command_t search_commands[] = {
    {
        .duration_us = 30000,
        .motion      = CONTROLLER_MOTION_ROTATION,
        .power       = +60,
    },
    {
        .duration_us = 60000,
        .motion      = CONTROLLER_MOTION_ROTATION,
        .power       = -60,
    },
    {
        .duration_us = 30000,
        .motion      = CONTROLLER_MOTION_ROTATION,
        .power       = +60,
    },
};

static const controller_sequence_t search_sequence = {
    .commands = search_commands,
    .length   = sizeof(search_commands) / sizeof(search_commands[0]),
    .repeat   = true,
};

void search_entry(void) {
    controller_stop();
}

void search_exit(void) {
    controller_stop();
}

void search_run(void) {
    if (ir_get_state() != IR_STATE_START) {
        return fsm_transition(FSM_STATE_SAFE);
    }

    if (sensor_detected_line()) {
        return fsm_transition(FSM_STATE_SURVIVE);
    }

    if (!sensor_detected_obstacle_sides()) {
        return fsm_transition(FSM_STATE_AUTONOMOUS);
    }

    if (controller_get_state() == CONTROLLER_STATE_IDLE) {
        if (!controller_start(&search_sequence)) {
            return;
        }
    }

    controller_step();
}
