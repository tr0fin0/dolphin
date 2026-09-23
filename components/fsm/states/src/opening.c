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
    .state = OPENING_STATE_SELECTION,
    .states_names = {
        [OPENING_STATE_EXECUTION] = "EXECUTING",
        [OPENING_STATE_FINISHED]  = "FINISHED",
        [OPENING_STATE_RELEASE]   = "RELEASE",
        [OPENING_STATE_SELECTION] = "SELECTING",
    },
    .step = 0,
    .strategy = OPENING_STATIC,
    .last_button = 0,
};

/**
 * @brief Opening strategy selection depending on radio receiver model.
 *
 * Available radio receiver models are:
 *
 * - `FS-GT2`: iterative measures of a single radio receiver channel.
 */
static void opening_selection(void) {
    pwm_norm_t current_button   = radio_read_channel(RADIO_CHANNEL_3);
    pwm_norm_t current_throttle = radio_read_channel(RADIO_CHANNEL_2);

    // ensure initial button value is not PWM_NEUTRAL_US
    if (
        (current_button != PWM_NEUTRAL_US) &&
        (opening_handler.last_button == PWM_NEUTRAL_US)
    ) {
        opening_handler.last_button = current_button;
    }

    // opening selection via sequential throttle value measures
    if (opening_handler.last_button != current_button) {
        opening_handler.step++;
        opening_handler.last_button = current_button;
        led_set_toggle(LED_STATE, 100);

        uint8_t increase = 2;
        if (current_throttle > (PWM_NEUTRAL_US+PWM_MAXIMUM_US)/2) increase = 3;
        if (current_throttle < (PWM_NEUTRAL_US+PWM_MINIMUM_US)/2) increase = 1;

        opening_handler.code = (
            (opening_code_t) (10 * opening_handler.code + increase)
        );
        LOG_I("opening strategy code is %d", opening_handler.code);
    }

    if (opening_handler.step == OPENING_ITERATIONS) {
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
            pwm_norm_t current_button = radio_read_channel(RADIO_CHANNEL_3);

            if (opening_handler.last_button != current_button) {
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
    for (uint8_t i = 0; i < NUMBER_OF_OPENINGS; i++) {
        if (opening_handler.code == opening_handler.strategies[i].code) {
            opening_handler.strategy = (opening_t) i;
            LOG_I(
                "opening strategy selected is %s",
                opening_manager.openings[i].name
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

opening_state_t opening_get_status(void) {
    return opening_handler.state;
}

void opening_run(void) {
    switch (CONFIG_CONTROL_MODE) {
        case CONFIG_CONTROL_AUTONOMOUS:
            break;

        case CONFIG_CONTROL_RADIO:
            if (radio_get_status() == RADIO_DISCONNECTED) {
                fsm_transition(STATE_SAFE);
            }
            break;

        default:
            break;
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
