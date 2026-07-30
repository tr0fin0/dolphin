#include "driver/gpio.h"
#include "encoder.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "logging.h"
#include <math.h>
#include "portmacro.h"
#include "pinout.h"
#include "pwm.h"
#include "soc/gpio_reg.h"

static volatile encoder_config_t encoders[NUMBER_OF_ENCODERS] = {
    [ENCODER_L] = {
        .name = "ENCODER_L",
        .pin  = PIN_ENCODER_L_A
    },
    [ENCODER_R] = {
        .name = "ENCODER_R",
        .pin  = PIN_ENCODER_R_A
    }
};

/**
 * @brief GPIO Interrupt Service Routine for Encoder PWM pulse capture.
 *
 * ISR attached to each encoder PWM pins and triggered on any edge.
 *
 * It measures the high-level pulse width in microseconds of a PWM signal
 * using edge timing.
 *
 * @param arg Encoder position encoded as void*.
 *
 * @note
 * GPIO level is read with direct ESP32 register access `REG_READ(GPIO_IN_REG)`
 * providing faster and ISR-safe input sampling.
 */
static void IRAM_ATTR encoder_isr(void *arg) {
    encoder_t encoder = (encoder_t) (uintptr_t) arg;

    uint32_t level = (REG_READ(GPIO_IN_REG) >> (encoders[encoder].pin)) & 0x1;

    int64_t now = esp_timer_get_time();
    if (level) { // rising  edge
        encoders[encoder].rise_time_us = now;
    } else {     // falling edge
        if (encoders[encoder].rise_time_us != 0) {
            pwm_t pwm = (pwm_t) (now - encoders[encoder].rise_time_us);

            encoders[encoder].pwm = pwm;
        }
    }
}

void encoder_init(void) {
    esp_err_t ret;

    ret = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    if (ret != (ESP_OK | ESP_ERR_INVALID_STATE)) {
        LOG_E(
            "%s and %s ISR install service failed with error %s.",
            encoders[ENCODER_L].name,
            encoders[ENCODER_R].name,
            esp_err_to_name(ret)
        );

        return;
    }

    int64_t now = esp_timer_get_time();
    for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        gpio_config_t pin_config = {
            .pin_bit_mask   = (1ULL << encoders[i].pin),
            .mode           = GPIO_MODE_INPUT,
            .pull_up_en     = GPIO_PULLUP_DISABLE,
            .pull_down_en   = GPIO_PULLDOWN_DISABLE,
            .intr_type      = GPIO_INTR_ANYEDGE
        };

        ret = gpio_config(&pin_config);
        if (ret != ESP_OK) {
            LOG_E(
                "%s GPIO configuration failed with error %s.",
                encoders[i].name,
                esp_err_to_name(ret)
            );

            return;
        }

        ret = gpio_isr_handler_add(
            encoders[i].pin,
            encoder_isr,
            (void *) (uintptr_t) i
        );
        if (ret != ESP_OK) {
            LOG_E(
                "%s ISR handler addition failed with error %s.",
                encoders[i].name,
                esp_err_to_name(ret)
            );

            return;
        }

        encoders[i].pwm = 0;
        encoders[i].rise_time_us = now;

        LOG_I(
            "%s initialized on pin %02d.",
            encoders[i].name,
            encoders[i].pin
        );
    }
}

const char *encoder_get_name(encoder_t encoder) {
    return encoders[encoder].name;
}

encoder_ang_rad_t encoder_read_angle(encoder_t encoder) {
    portDISABLE_INTERRUPTS();
    pwm_t pwm = encoders[encoder].pwm;
    portENABLE_INTERRUPTS();

    encoder_ang_rad_t angle_rad = (
        (encoder_ang_rad_t) M_TWOPI * (
            ((float) pwm        - ENCODER_PWM_MIN_US) /
            (ENCODER_PWM_MAX_US - ENCODER_PWM_MIN_US)
        )
    );

    if (angle_rad < 0.0f)    angle_rad = 0.0f;
    if (angle_rad > M_TWOPI) angle_rad = M_TWOPI;

    LOG_V(
        "%s measured angle is %.4f rad.",
        encoders[encoder].name,
        angle_rad
    );

    return angle_rad;
}
