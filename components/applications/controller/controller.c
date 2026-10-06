#include "controller.h"
#include "esc.h"
#include "esp_timer.h"
#include "logging.h"

static controller_t controller = {
    .name = "controller",
    .states_names = {
        [CONTROLLER_STATE_ACTIVE] = "CONTROLLER_ACTIVE",
        [CONTROLLER_STATE_IDLE]   = "CONTROLLER_IDLE",
    },
};

/**
 * @brief Set ESCs commands.
 *
 * The power signals determine the movement type:
 *
 * - ``rotation``: if powers have **equal** signs
 *
 * - ``translation``: if powers have **different** signs
 *
 * @param[in] power_l PWM percentage for @ref ESC_L.
 * @param[in] power_r PWM percentage for @ref ESC_R.
 */
static void controller_set_movement(
    pwm_percentage_t power_l, pwm_percentage_t power_r
) {
    esc_set_pwm(ESC_L, pwm_percentage(power_l));
    esc_set_pwm(ESC_R, pwm_percentage(power_r));
}

/**
 * @brief Set controller command.
 *
 * @param[in] command Command passed by reference.
 */
static void controller_set_command(const controller_command_t *command) {
    if (command == NULL) {
        LOG_E("controller *command is NULL");

        return;
    }

    switch (command->motion) {
        case CONTROLLER_MOTION_ROTATION:
            if (command->power > 0) {
                controller_set_movement(
                    +command->power,
                    +command->power
                );
            } else if (command->power < 0) {
                controller_set_movement(
                    -command->power,
                    -command->power
                );
            } else {
                esc_set_pwm_mix_neutral();
            }
            break;

        case CONTROLLER_MOTION_TRANSLATION:
            if (command->power > 0) {
                controller_set_movement(
                    +command->power,
                    -command->power
                );
            } else if (command->power < 0) {
                controller_set_movement(
                    +command->power,
                    -command->power
                );
            } else {
                esc_set_pwm_mix_neutral();
            }
            break;

        case CONTROLLER_MOTION_STOP:
            esc_set_pwm_mix_neutral();
            break;

        default:
            esc_set_pwm_mix_neutral();
            break;
    }
}

controller_state_t controller_get_state(void) {
    return controller.state;
}

const char *controller_get_state_name(void) {
    if (controller.state >= NUMBER_OF_CONTROLLER_STATES) {
        return NULL;
    }

    return controller.states_names[controller.state];
}

void controller_init(void) {
    controller_stop();
}

bool controller_start(const controller_sequence_t *sequence) {
    if (controller.state != CONTROLLER_STATE_IDLE) {
        LOG_E("%s invalid controller state", controller_get_state_name());

        return false;
    }

    if (sequence == NULL) {
        LOG_E("controller *sequence is NULL");

        return false;
    }

    if (sequence->length > 0 && sequence->commands == NULL) {
        LOG_E(
            "controller *sequence=%p of length %u with *commands NULL",
            (void *)sequence,
            sequence->length
        );

        return false;
    }

    controller.command_current  = 0;

    if (sequence->length == 0) {
        LOG_I(
            "controller *sequence=%p of length %u with *commands empty",
            (void *)sequence,
            sequence->length
        );
        controller.sequence = NULL;

        return true;
    }

    LOG_I(
        "starting controller *sequence=%p of length %u with *commands=%p",
        (void *)sequence,
        sequence->length,
        (void *)sequence->commands
    );

    controller.command_start_us = esp_timer_get_time();
    controller.sequence         = sequence;
    controller.state            = CONTROLLER_STATE_ACTIVE;

    controller_set_command(
        &controller.sequence->commands[controller.command_current]
    );

    return true;
}

void controller_step(void) {
    if (controller.state != CONTROLLER_STATE_ACTIVE) {
        LOG_E("%s invalid controller state", controller_get_state_name());

        return;
    }

    if (controller.sequence == NULL) {
        LOG_E("controller *sequence is NULL");

        return;
    }

    if (controller.sequence->commands == NULL) {
        LOG_E(
            "controller *sequence=%p of length %u with *commands NULL",
            (void *)controller.sequence,
            controller.sequence->length
        );

        return;
    }

    const controller_command_t *command = &controller.sequence->commands[
        controller.command_current
    ];

    const int64_t now_us = esp_timer_get_time();

    if ((now_us - controller.command_start_us) < command->duration_us) {
        return;
    }

    // current command has finished
    controller.command_current++;
    if (controller.command_current >= controller.sequence->length) {
        if (controller.sequence->repeat) {
            LOG_I(
                "repeating controller *sequence=%p of length %u with *commands=%p",
                (void *)controller.sequence,
                controller.sequence->length,
                (void *)controller.sequence->commands
            );

            controller.command_current = 0;
            controller.command_start_us = now_us;

            controller_set_command(
                &controller.sequence->commands[
                    controller.command_current
                ]
            );

            LOG_I(
                "command=%p sequence=%p commands=%p index=%u",
                (void *)command,
                (void *)controller.sequence,
                (void *)controller.sequence->commands,
                controller.command_current
            );

            return;
        }

        return controller_stop();
    }

    // start next command
    controller.command_start_us += command->duration_us;
    controller_set_command(
        &controller.sequence->commands[controller.command_current]
    );
}

void controller_stop(void) {
    LOG_I("stopping controller");

    esc_set_pwm_mix_neutral();

    controller.command_current  = 0;
    controller.sequence         = NULL;
    controller.state            = CONTROLLER_STATE_IDLE;
}
