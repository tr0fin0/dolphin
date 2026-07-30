#include "fsm.h"
#include "logging.h"
#include "opening.h"
#include "radio.h"

static opening_code_t opening_code;

static opening_config_t openings[NUMBER_OF_OPENINGS] = {
    [OPENING_STATIC] = {
        .name       = "STATIC",
        .code       = 222,
        .waypoints  = {
            +0.0, +0.0, DEG2RAD(0),    0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_DRAW] = {
        .name       = "DRAW",
        .code       = 221,
        .waypoints  = {
            +0.0, +0.0, DEG2RAD(180),  0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_NE] = {
        .name       = "NORTH-EAST",
        .code       = 331,
        .waypoints  = {
            +0.6, +0.6, DEG2RAD(-45),  0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_NN] = {
        .name       = "NORTH-NORTH",
        .code       = 232,
        .waypoints  = {
            +0.0, +0.6, DEG2RAD(0),    0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_NW] = {
        .name       = "NORTH-WEST",
        .code       = 133,
        .waypoints  = {
            -0.6, +0.6, DEG2RAD(+45),  0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_SEN] = {
        .name       = "SOUTH-EAST-NEUTRAL",
        .code       = 312,
        .waypoints  = {
            +0.6, -0.6, DEG2RAD(-45),  0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_SE] = {
        .name       = "SOUTH-EAST",
        .code       = 313,
        .waypoints  = {
            +0.6, -0.6, DEG2RAD(+90),  0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_SS] = {
        .name       = "SOUTH-SOUTH",
        .code       = 212,
        .waypoints  = {
            +0.0, -0.6, DEG2RAD(0),    0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_SW] = {
        .name       = "SOUTH-WEST",
        .code       = 111,
        .waypoints  = {
            -0.6, -0.6, DEG2RAD(-90),  0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    },
    [OPENING_SWN] = {
        .name       = "SOUTH-WEST-NEUTRAL",
        .code       = 112,
        .waypoints  = {
            -0.6, -0.6, DEG2RAD(+90),  0.05, DEG2RAD(+90), 0.05, DEG2RAD(+5)
        }
    }
};

static pwm_norm_t last_button;

opening_step_t opening_step = 0;
opening_t opening_strategy  = OPENING_STATIC;

/**
 * @brief Opening strategy selection depending on radio receiver model.
 *
 * Available radio receiver models are:
 *
 * - `FS-GT2`: iterative measures of a single radio receiver channel.
 */
static void opening_selection(void) {
    pwm_norm_t current_button   = radio_read_channel(RADIO_CHANNEL_BUTTON);
    pwm_norm_t current_throttle = radio_read_channel(RADIO_CHANNEL_THROTTLE);

    // ensure initial button value is not PWM_NEUTRAL_US
    if (last_button == PWM_NEUTRAL_US && current_button != PWM_NEUTRAL_US) {
        last_button = current_button;
    }

    // opening selection via sequential throttle value measures
    if (last_button != current_button) {
        opening_step++;
        last_button = current_button;
        led_set_toggle(LED_STATE, 100);

        uint8_t increase = 2;
        if (current_throttle > (PWM_NEUTRAL_US+PWM_MAXIMUM_US)/2) increase = 3;
        if (current_throttle < (PWM_NEUTRAL_US+PWM_MINIMUM_US)/2) increase = 1;

        opening_code = (opening_code_t) (10 * opening_code + increase);
        LOG_I("opening strategy code is %d", opening_code);
    }
}

/**
 * @brief Opening strategy wait release command from radio receiver.
 *
 * Opening strategy is executed upon user confirmation.
 */
static void opening_release(void) {
    pwm_norm_t current_button = radio_read_channel(RADIO_CHANNEL_BUTTON);
    if (last_button != current_button) {
        opening_step++;

        for (uint8_t i = 0; i < NUMBER_OF_OPENINGS; i++) {
            if (opening_code == openings[i].code) {
                opening_strategy = (opening_t) i;
                LOG_I("opening strategy selected is %s", openings[i].name);

                break;
            }
        }
    }
}

/**
 * @brief Opening strategy execution.
 */
static void opening_execution(void) {

}

void opening_entry(void) {
    last_button  = radio_read_channel(RADIO_CHANNEL_BUTTON);
}

void opening_run(void) {
    if (radio_status() == RADIO_DISCONNECTED) {
        fsm_transition(STATE_SAFE);

        return;
    }

    if (opening_step < OPENING_ITERATIONS) {
        opening_selection();
    } else if (opening_step == OPENING_ITERATIONS) {
        opening_release();
    } else {
        opening_execution();
    }
}
