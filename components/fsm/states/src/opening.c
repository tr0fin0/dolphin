#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "ir.h"
#include "led.h"
#include "logging.h"
#include "opening.h"
#include "radio.h"

static opening_handler_t opening_handler = {
    .name = "Opening Handler",
    .codes_names = {
        [OPENING_CODE_H] = "CODE_H",
        [OPENING_CODE_L] = "CODE_L",
        [OPENING_CODE_N] = "CODE_N",
    },
    .state = OPENING_STATE_SELECTION,
    .states_names = {
        [OPENING_STATE_EXECUTION] = "EXECUTING",
        [OPENING_STATE_FINISHED]  = "FINISHED",
        [OPENING_STATE_RELEASE]   = "RELEASE",
        [OPENING_STATE_SELECTION] = "SELECTING",
    },
    .step = OPENING_STEP_0,
    .strategy = OPENING_STATIC,
    .strategies = {
        [OPENING_STATIC] = {
            .name = "STATIC",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_N,
                [OPENING_STEP_2] = OPENING_CODE_N,
            }
        },
        [OPENING_DRAW]   = {
            .name = "DRAW",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_L,
                [OPENING_STEP_1] = OPENING_CODE_N,
                [OPENING_STEP_2] = OPENING_CODE_N,
            }
        },
        [OPENING_NE]     = {
            .name = "NORTH-EAST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_L,
                [OPENING_STEP_1] = OPENING_CODE_H,
                [OPENING_STEP_2] = OPENING_CODE_H,
            }
        },
        [OPENING_NN]     = {
            .name = "NORTH-NORTH",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_H,
                [OPENING_STEP_2] = OPENING_CODE_N,
            }
        },
        [OPENING_NW]     = {
            .name = "NORTH-WEST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_H,
                [OPENING_STEP_1] = OPENING_CODE_H,
                [OPENING_STEP_2] = OPENING_CODE_L,
            }
        },
        [OPENING_SEN]    = {
            .name = "SOUTH-EAST-NEUTRAL",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_H,
            }
        },
        [OPENING_SE]     = {
            .name = "SOUTH-EAST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_H,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_H,
            }
        },
        [OPENING_SS]     = {
            .name = "SOUTH-SOUTH",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_N,
            }
        },
        [OPENING_SW]     = {
            .name = "SOUTH-WEST",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_L,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_L,
            }
        },
        [OPENING_SWN]    = {
            .name = "SOUTH-WEST-NEUTRAL",
            .code = {
                [OPENING_STEP_0] = OPENING_CODE_N,
                [OPENING_STEP_1] = OPENING_CODE_L,
                [OPENING_STEP_2] = OPENING_CODE_L,
            }
        },
    },
    .last_button = OPENING_INITIAL_BUTTON,
};

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
        opening_handler.state = OPENING_STATE_RELEASE;

        led_set_color(LED_STATE, LED_COLOR_BLUE_LIGHT);
    }
}

/**
 * @brief Opening strategy wait release command from radio receiver.
 *
 * Opening strategy is executed upon user confirmation.
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
 * @brief Opening strategy execution.
 */
static void opening_execution(void) {
    for (opening_t strategy = 0; strategy < NUMBER_OF_OPENINGS; strategy++) {
        if (
            memcmp(
                opening_handler.code,
                opening_handler.strategies[strategy].code,
                sizeof(opening_handler.code)
            ) == 0
        ) {
            opening_handler.strategy = strategy;

            LOG_I(
                "opening strategy selected is %s",
                opening_get_strategy_name(opening_handler.strategy)
            );

            break;
        }
    }

    switch (opening_handler.strategy) {
        case OPENING_STATIC:
            break;

        case OPENING_DRAW:
            // rotation     +180
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(80);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_NE:
            // rotation     +045
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(25);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // translation  +50
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(140);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // rotation     -090
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(65);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_NN:
            // translation  +50
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(140);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_NW:
            // rotation     -045
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(25);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // translation  +50
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(140);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // rotation     +090
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(65);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_SE:
            // rotation     -045
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(30);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // translation  -50
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(120);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // rotation     +090
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(60);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_SEN:
            // rotation     -045
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(30);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // translation  -50
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(120);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_SS:
            // translation  -50
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(120);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_SW:
            // rotation     +045
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(30);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // translation  -50
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(120);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // rotation     -090
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(-90), ESC_R);
            // // delay(60);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        case OPENING_SWN:
            // rotation     +045
            // esc_set_pwm(pwm_percentage(+90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(20);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            // translation  -50
            // esc_set_pwm(pwm_percentage(-90), ESC_L);
            // esc_set_pwm(pwm_percentage(+90), ESC_R);
            // // delay(120);
            // esc_set_pwm_mix_neutral();
            // // delay(1);

            break;

        default:
            break;
    }

    opening_handler.state = OPENING_STATE_FINISHED;
}

void opening_entry(void) {
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
        fsm_transition(STATE_SAFE);
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
