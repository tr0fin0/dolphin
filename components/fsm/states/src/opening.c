#include "config.h"
#include "esc.h"
#include "fsm.h"
#include "ir.h"
#include "led.h"
#include "logging.h"
#include "opening.h"
#include "radio.h"

/**
 * @brief 
 */
static opening_manager_t opening_manager = {
    .name = "Opening Manager",
    .openings = {
        [OPENING_STATIC] = { .name = "STATIC",              .code = 222, },
        [OPENING_DRAW]   = { .name = "DRAW",                .code = 221, },
        [OPENING_NE]     = { .name = "NORTH-EAST",          .code = 331, },
        [OPENING_NN]     = { .name = "NORTH-NORTH",         .code = 232, },
        [OPENING_NW]     = { .name = "NORTH-WEST",          .code = 133, },
        [OPENING_SEN]    = { .name = "SOUTH-EAST-NEUTRAL",  .code = 312, },
        [OPENING_SE]     = { .name = "SOUTH-EAST",          .code = 313, },
        [OPENING_SS]     = { .name = "SOUTH-SOUTH",         .code = 212, },
        [OPENING_SW]     = { .name = "SOUTH-WEST",          .code = 111, },
        [OPENING_SWN]    = { .name = "SOUTH-WEST-NEUTRAL",  .code = 112, },
    },
    .status = OPENING_STATUS_SELECTION,
    .status_names = {
        [OPENING_STATUS_EXECUTION] = "EXECUTING",
        [OPENING_STATUS_FINISHED]  = "FINISHED",
        [OPENING_STATUS_SELECTION] = "SELECTING",
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
        (opening_manager.last_button == PWM_NEUTRAL_US)
    ) {
        opening_manager.last_button = current_button;
    }

    // opening selection via sequential throttle value measures
    if (opening_manager.last_button != current_button) {
        opening_manager.step++;
        opening_manager.last_button = current_button;
        led_set_toggle(LED_STATE, 100);

        uint8_t increase = 2;
        if (current_throttle > (PWM_NEUTRAL_US+PWM_MAXIMUM_US)/2) increase = 3;
        if (current_throttle < (PWM_NEUTRAL_US+PWM_MINIMUM_US)/2) increase = 1;

        opening_manager.code = (
            (opening_code_t) (10 * opening_manager.code + increase)
        );
        LOG_I("opening strategy code is %d", opening_manager.code);
    }

    if (opening_manager.step == OPENING_ITERATIONS) {
        opening_manager.status = OPENING_STATUS_RELEASE;

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
                opening_manager.status = OPENING_STATUS_EXECUTION;
            }
            break;

        case CONFIG_CONTROL_RADIO:
            pwm_norm_t current_button = radio_read_channel(RADIO_CHANNEL_3);

            if (opening_manager.last_button != current_button) {
                opening_manager.status = OPENING_STATUS_EXECUTION;
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
        if (opening_manager.code == opening_manager.openings[i].code) {
            opening_manager.strategy = (opening_t) i;
            LOG_I(
                "opening strategy selected is %s",
                opening_manager.openings[i].name
            );
            
            break;
        }
    }

    switch (opening_manager.strategy) {
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

    opening_manager.status = OPENING_STATUS_FINISHED;
}

void opening_entry(void) {
    opening_manager.last_button = radio_read_channel(RADIO_CHANNEL_3);
}

opening_status_t opening_get_status(void) {
    return opening_manager.status;
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

    switch (opening_manager.status) {
        case OPENING_STATUS_EXECUTION:
            opening_execution();
            break;

        case OPENING_STATUS_FINISHED:
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

        case OPENING_STATUS_RELEASE:
            opening_release();
            break;

        case OPENING_STATUS_SELECTION:
            opening_selection();
            break;

        default:
            break;
    }
}
