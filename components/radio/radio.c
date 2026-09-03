#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "logging.h"
#include "portmacro.h"
#include "pinout.h"
#include "pwm.h"
#include "radio.h"
#include "soc/gpio_reg.h"

static volatile radio_t radio = {
    .name = "FS-GT2",
    .status_names = {
        [RADIO_DISCONNECTED] = "DISCONNECTED",
        [RADIO_CONNECTED]    = "CONNECTED",
    },
    .connections = {
        [RADIO_CHANNEL_1] = { .name = "STEERING",   .pin = PIN_RC_CH1 },
        [RADIO_CHANNEL_2] = { .name = "THROTTLE",   .pin = PIN_RC_CH2 },
        [RADIO_CHANNEL_3] = { .name = "BUTTON",     .pin = PIN_RC_CH3 },
        [RADIO_CHANNEL_4] = { .name = "CHANNEL_4",  .pin = PIN_RC_CH4 },
        [RADIO_CHANNEL_5] = { .name = "CHANNEL_5",  .pin = PIN_RC_CH5 },
        [RADIO_CHANNEL_6] = { .name = "CHANNEL_6",  .pin = PIN_RC_CH6 },
    },
};

/**
 * @brief GPIO Interrupt Service Routine for Radio Controller PWM pulse capture.
 *
 * ISR attached to each Radio Receiver channel pins and triggered on any edge.
 *
 * It measures the high-level pulse width in microseconds of a PWM signal
 * using edge timing.
 *
 * @param arg Radio Receiver channel encoded as void*.
 *
 * @note
 * GPIO level is read with direct ESP32 register access `REG_READ(GPIO_IN_REG)`
 * providing faster and ISR-safe input sampling.
 */
static void IRAM_ATTR radio_isr(void *arg) {
    radio_channel_t channel = (radio_channel_t) (uintptr_t) arg;

    uint32_t level = (
        REG_READ(GPIO_IN1_REG) >> (radio.connections[channel].pin - 32)
    ) & 0x1;

    int64_t now = esp_timer_get_time();
    if (level) { // rising  edge
        radio.connections[channel].rise_time_us = now;
    } else {     // falling edge
        if (radio.connections[channel].rise_time_us != 0) {
            pwm_t pwm = (pwm_t) (now - radio.connections[channel].rise_time_us);

            radio.connections[channel].pwm = pwm_normalize(pwm);
            radio.connections[channel].last_time_us = now;
        }
    }
}

const char *radio_get_channel_name(radio_channel_t channel) {
    return radio.connections[channel].name;
}

const char *radio_get_name(void) {
    return radio.name;
}

radio_status_t radio_get_status() {
    portDISABLE_INTERRUPTS();
    int64_t now = esp_timer_get_time();

    bool steering_dead = (
        now - radio.connections[RADIO_CHANNEL_1].last_time_us
    ) > RADIO_TIMEOUT_US;
    bool throttle_dead = (
        now - radio.connections[RADIO_CHANNEL_2].last_time_us
    ) > RADIO_TIMEOUT_US;
    portENABLE_INTERRUPTS();

    radio_status_t new_status;
    if (steering_dead || throttle_dead) {
        new_status = RADIO_DISCONNECTED;
    } else {
        new_status = RADIO_CONNECTED;
    }

    if (new_status != radio.status) {
        radio.status = new_status;

        LOG_W("%s radio %s", radio_get_name(), radio_get_status_name());
    }

    return radio.status;
}

const char *radio_get_status_name(void) {
    return radio.status_names[radio.status];
}

void radio_init() {
    esp_err_t ret;

    ret = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    if (ret != (ESP_OK | ESP_ERR_INVALID_STATE)) {
        LOG_E(
            "%s ISR install service failed with error %s.",
            radio_get_name(),
            esp_err_to_name(ret)
        );

        return;
    }

    int64_t now = esp_timer_get_time();
    for (uint8_t channel = 0; channel < NUMBER_OF_RADIO_CHANNELS; channel++) {
        gpio_config_t pin_config = {
            .pin_bit_mask   = (1ULL << radio.connections[channel].pin),
            .mode           = GPIO_MODE_INPUT,
            .pull_up_en     = GPIO_PULLUP_DISABLE,
            .pull_down_en   = GPIO_PULLDOWN_DISABLE,
            .intr_type      = GPIO_INTR_ANYEDGE
        };

        ret = gpio_config(&pin_config);
        if (ret != ESP_OK) {
            LOG_E(
                "%s %s GPIO configuration failed with error %s.",
                radio_get_name(),
                radio_get_channel_name(channel),
                esp_err_to_name(ret)
            );

            return;
        }

        ret = gpio_isr_handler_add(
            radio.connections[channel].pin,
            radio_isr,
            (void *) (uintptr_t) channel
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s %s ISR handler addition failed with error %s.",
                radio_get_name(),
                radio_get_channel_name(channel),
                esp_err_to_name(ret)
            );

            return;
        }

        radio.connections[channel].pwm = (pwm_t) PWM_NEUTRAL_US;
        radio.connections[channel].rise_time_us = now;
        radio.connections[channel].last_time_us = now;

        LOG_I(
            "%s %s initialized on pin %02d.",
            radio_get_name(),
            radio_get_channel_name(channel),
            radio.connections[channel].pin
        );
    }

    radio.status = RADIO_DISCONNECTED;
}

pwm_norm_t radio_read_channel(radio_channel_t channel) {
    pwm_norm_t pwm;

    portDISABLE_INTERRUPTS();
    pwm = radio.connections[channel].pwm;
    portENABLE_INTERRUPTS();

    LOG_V(
        "%s %s PWM is %04d us.",
        radio_get_name(),
        radio_get_channel_name(channel),
        radio.connections[channel].pwm
    );

    return pwm;
}

void radio_read_channels(pwm_norm_t *pwms) {
    portDISABLE_INTERRUPTS();
    for (uint8_t channel = 0; channel < NUMBER_OF_RADIO_CHANNELS; channel++) {
        pwms[channel] = radio.connections[channel].pwm;
    }
    portENABLE_INTERRUPTS();
}
