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

static volatile radio_t radio_receiver = {
    .name = "FS-GT2",
    .pins = {
        PIN_RADIO_CH1,
        PIN_RADIO_CH2,
        PIN_RADIO_CH3,
        PIN_RADIO_CH4,
        PIN_RADIO_CH5,
        PIN_RADIO_CH6
    }
};

/**
 * @brief GPIO Interrupt Service Routine for Radio Controller PWM pulse capture.
 *
 * ISR attached to each Radio Receiver channel pins and triggered on any edge.
 *
 * It measures the high-level pulse width in microseconds of a PWM signal
 * using edge timing.
 *
 * @param arg  Radio Receiver channel encoded as void*.
 *
 * @note
 * GPIO level is read with direct ESP32 register access `REG_READ(GPIO_IN_REG)`
 * providing faster and ISR-safe input sampling.
 */
static void IRAM_ATTR radio_isr(void *arg) {
    radio_channel_t channel = (uint8_t) (uintptr_t) arg; // portable cast

    uint32_t level = (
        REG_READ(GPIO_IN1_REG) >> (radio_receiver.pins[channel] - 32)
    ) & 0x1;

    int64_t now = esp_timer_get_time();
    if (level) { // rising edge
        radio_receiver.rise_times_us[channel] = now;
    } else { // falling edge
        if (radio_receiver.rise_times_us[channel] != 0) {
            pwm_t pwm = (pwm_t) (now - radio_receiver.rise_times_us[channel]);

            radio_receiver.pwms[channel] = pwm_normalize(pwm);
            radio_receiver.last_times_us[channel] = now;
        }
    }
}

void radio_init() {
    esp_err_t ret;

    ret = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    if (ret != ESP_OK) {
        LOG_E(
            "%s ISR install service failed with error %s.",
            radio_receiver.name,
            esp_err_to_name(ret)
        );

        return;
    }

    int64_t now = esp_timer_get_time();
    for (uint8_t channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
        gpio_config_t pin_config = {
            .pin_bit_mask   = (1ULL << radio_receiver.pins[channel]),
            .mode           = GPIO_MODE_INPUT,
            .pull_up_en     = GPIO_PULLUP_DISABLE,
            .pull_down_en   = GPIO_PULLDOWN_DISABLE,
            .intr_type      = GPIO_INTR_ANYEDGE
        };

        ret = gpio_config(&pin_config);
        if (ret != ESP_OK) {
            LOG_E(
                "%s channel %d GPIO configuration failed with error %s.",
                radio_receiver.name,
                radio_receiver.pins[channel],
                esp_err_to_name(ret)
            );

            return;
        }

        ret = gpio_isr_handler_add(
            radio_receiver.pins[channel],
            radio_isr,
            (void *) (uintptr_t) channel
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s channel %d ISR handler addition failed with error %s.",
                radio_receiver.name,
                radio_receiver.pins[channel],
                esp_err_to_name(ret)
            );

            return;
        }

        radio_receiver.pwms[channel] = (pwm_t) PWM_NEUTRAL_US;
        radio_receiver.rise_times_us[channel] = now;
        radio_receiver.last_times_us[channel] = now;

        LOG_I(
            "%s channel %d initialized on pin %d.",
            radio_receiver.name,
            channel,
            radio_receiver.pins[channel]
        );
    }

    radio_receiver.status = RADIO_DISCONNECTED;
}

void radio_read_channels(pwm_norm_t pwms[NUMBER_OF_CHANNELS]) {
    portDISABLE_INTERRUPTS();
    for (uint8_t channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
        pwms[channel] = radio_receiver.pwms[channel];
    }
    portENABLE_INTERRUPTS();
}

pwm_norm_t radio_read_channel(radio_channel_t channel) {
    pwm_norm_t pwm;

    portDISABLE_INTERRUPTS();
    pwm = radio_receiver.pwms[channel];
    portENABLE_INTERRUPTS();

    LOG_V(
        "%s channel %d: %d.",
        radio_receiver.name,
        channel,
        radio_receiver.pwms[channel]
    );

    return pwm;
}

radio_status_t radio_status() {
    portDISABLE_INTERRUPTS();
    int64_t now = esp_timer_get_time();

    bool steering_dead = (
        now - radio_receiver.last_times_us[CHANNEL_STEERING]
    ) > RADIO_TIMEOUT_US;
    bool throttle_dead = (
        now - radio_receiver.last_times_us[CHANNEL_THROTTLE]
    ) > RADIO_TIMEOUT_US;
    portENABLE_INTERRUPTS();

    radio_status_t new_status;
    if (steering_dead || throttle_dead) {
        new_status = RADIO_DISCONNECTED;
    } else {
        new_status = RADIO_CONNECTED;
    }

    if (new_status != radio_receiver.status) {
        radio_receiver.status = new_status;

        LOG_W(
            "%s radio status is %d.",
            radio_receiver.name,
            radio_receiver.status
        );
    }

    return radio_receiver.status;
}
