#include "esc.h"
#include "esp_err.h"
#include "logging.h"

static esc_config_t controllers[NUMBER_OF_ESCS] = {
    [ESC_L] = {
        .name       = "ESC_L",
        .pin        = PIN_ESC_L,
        .comparator = NULL
    },
    [ESC_R] = {
        .name       = "ESC_R",
        .pin        = PIN_ESC_R,
        .comparator = NULL
    }
};

void esc_init() {
    esp_err_t ret;

    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .count_mode     = MCPWM_TIMER_COUNT_MODE_UP,
        .clk_src        = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .group_id       = ESC_GROUP_ID,
        .period_ticks   = ESC_PERIOD_US,
        .resolution_hz  = ESC_RESOLUTION_HZ,
    };

    ret = mcpwm_new_timer(&timer_config, &timer);
    if (ret != ESP_OK) {
        LOG_E(
            "MCPWM timer creation failed with error %s.", esp_err_to_name(ret)
        );

        return;
    }

    mcpwm_oper_handle_t operator = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = ESC_GROUP_ID,
    };
    ret = mcpwm_new_operator(&operator_config, &operator);
    if (ret != ESP_OK) {
        LOG_E(
            "MCPWM operator creation failed with error %s.",
            esp_err_to_name(ret)
        );

        return;
    }

    ret = mcpwm_operator_connect_timer(operator, timer);
    if (ret != ESP_OK) {
        LOG_E(
            "MCPWM timer connection to operator failed with error %s.",
            esp_err_to_name(ret)
        );

        return;
    }

    for (uint8_t i = 0; i < NUMBER_OF_ESCS; i++) {
        mcpwm_comparator_config_t comparator_config = {
            .flags.update_cmp_on_tez = true,
        };
        ret = mcpwm_new_comparator(
            operator, &comparator_config, &controllers[i].comparator
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s MCPWM comparator creation failed with error %s.",
                controllers[i].name, esp_err_to_name(ret)
            );

            return;
        }

        mcpwm_gen_handle_t generator = NULL;
        mcpwm_generator_config_t generator_config = {
            .gen_gpio_num = controllers[i].pin,
        };
        ret = mcpwm_new_generator(operator, &generator_config, &generator);
        if (ret != ESP_OK) {
            LOG_E(
                "%s MCPWM generator creation failed with error %s",
                controllers[i].name, esp_err_to_name(ret)
            );

            return;
        };

        ret = mcpwm_generator_set_action_on_timer_event(
            generator,
            MCPWM_GEN_TIMER_EVENT_ACTION(
                MCPWM_TIMER_DIRECTION_UP,
                MCPWM_TIMER_EVENT_EMPTY,
                MCPWM_GEN_ACTION_HIGH
            )
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s MCPWM generator set action on timer failed with error %s",
                controllers[i].name, esp_err_to_name(ret)
            );

            return;
        };

        ret = mcpwm_generator_set_action_on_compare_event(
            generator,
            MCPWM_GEN_COMPARE_EVENT_ACTION(
                MCPWM_TIMER_DIRECTION_UP,
                controllers[i].comparator,
                MCPWM_GEN_ACTION_LOW
            )
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s MCPWM generator set action on compare failed with error %s",
                controllers[i].name, esp_err_to_name(ret)
            );

            return;
        };

        ret = mcpwm_comparator_set_compare_value(
            controllers[i].comparator, PWM_NEUTRAL_US
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s compare value set failed with error %s.",
                controllers[i].name, esp_err_to_name(ret)
            );

            return;
        }

        LOG_I(
            "%s initialized on pin %d.", controllers[i].name, controllers[i].pin
        );
    }

    ret = mcpwm_timer_enable(timer);
    if (ret != ESP_OK) {
        LOG_E("MCPWM timer enable failed with error %s.", esp_err_to_name(ret));

        return;
    }
    ret = mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP);
    if (ret != ESP_OK) {
        LOG_E("MCPWM timer start failed with error %s.", esp_err_to_name(ret));

        return;
    }
};

void esc_set_pwm(esc_t esc, pwm_t pwm) {
    pwm_norm_t pwm_norm = pwm_normalize(pwm);

    esp_err_t ret = mcpwm_comparator_set_compare_value(
        controllers[esc].comparator, pwm_norm
    );
    if (ret != ESP_OK) {
        LOG_E(
            "%s compare value set failed with error %s.",
            controllers[esc].name, esp_err_to_name(ret)
        );

        return;
    }

    LOG_V("%s duty cycle set to %d us.", controllers[esc].name, pwm_norm);
};

void esc_set_pwm_mix(pwm_norm_t pwms[NUMBER_OF_ESCS]) {
    pwm_norm_t steering_us = pwms[0];
    pwm_norm_t throttle_us = pwms[1];

    esc_set_pwm(ESC_L, (pwm_t) steering_us + throttle_us - PWM_NEUTRAL_US);
    esc_set_pwm(ESC_R, (pwm_t) steering_us - throttle_us + PWM_NEUTRAL_US);
};

void esc_set_pwm_mix_neutral() {
    esc_set_pwm_neutral(ESC_L);
    esc_set_pwm_neutral(ESC_R);
};

void esc_set_pwm_neutral(esc_t esc) {
    esc_set_pwm(esc, (pwm_t)PWM_NEUTRAL_US);
};
