#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include "debug.h"
#include "pinout.h"
#include "pwm.h"
#include "radio.h"
#include "soc/gpio_reg.h"



static volatile uint32_t            rise_time[NUMBER_OF_CHANNELS];
static volatile pwm_pulse_norm_t    pulse_width[NUMBER_OF_CHANNELS];
static volatile uint32_t            last_update[NUMBER_OF_CHANNELS];
static radio_status_t               status = RADIO_DISCONNECTED;

static const uint8_t channels[NUMBER_OF_CHANNELS] = {
    PIN_RADIO_CH1,
    PIN_RADIO_CH2,
    PIN_RADIO_CH3,
    PIN_RADIO_CH4,
    PIN_RADIO_CH5,
    PIN_RADIO_CH6
};



/**
 * @brief GPIO Interrupt Service Routine for RC PWM pulse capture.
 *
 * This ISR is attached to each radio input pin and triggered on CHANGE.
 * It measures the high-level pulse width in microseconds of a PWM signal
 * using edge timing:
 *
 * - rising edge: stores the timestamp from `micros()` in `rise_time[channel]`.
 *
 * - falling edge: computes pulse duration as `now - rise_time[channel])`,
 *   normalizes it via `pwm_pulse_us_normalize()`, and stores the result in
 *   `pulse_width[channel]`. It also updates last_update[channel] with millis().
 *
 * The channel index is passed via the interrupt argument and recovered using
 * a portable uintptr_t cast.
 *
 * GPIO level is read using a direct ESP32 register access:
 * `REG_READ(GPIO_IN_REG)` which provides faster and ISR-safe input sampling
 * compared to digitalRead().
 *
 * @param arg  Channel index (0..NUMBER_OF_CHANNELS-1) encoded as void*.
 *
 * @note
 * - Marked IRAM_ATTR to ensure execution from IRAM (ESP32) and avoid flash
 *   access during interrupt context.
 *
 * - Execution time must remain minimal: no dynamic allocation, no logging,
 *   no blocking calls.
 *
 * - Shared variables (rise_time, pulse_width, last_update) are volatile
 *   and accessed atomically or within critical sections.
 */
static void IRAM_ATTR radio_isr(void* arg) {
    uint8_t i       = (uint8_t)(uintptr_t) arg; // portable cast

    uint32_t now    = micros();
    uint32_t level  = (REG_READ(GPIO_IN1_REG) >> (channels[i] - 32)) & 0x1;

    if (level) {    // rising edge
        rise_time[i] = now;
    } else {        // falling edge
        if (rise_time[i] != 0) {
            pwm_pulse_t pulse_us = (pwm_pulse_t) (now - rise_time[i]);

            pulse_width[i] = pwm_pulse_us_normalize(pulse_us);
            last_update[i] = now;
        }
    }
}



void radio_init() {
    LOG_I("initialization of Radio Controller started");

    uint32_t now = micros();
    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        pinMode(channels[i], INPUT);

        attachInterruptArg(
            digitalPinToInterrupt(channels[i]),
            radio_isr,
            (void*)(uintptr_t) i,
            CHANGE
        );

        rise_time[i]    = now;
        pulse_width[i]  = (pwm_pulse_t) PWM_NEUTRAL_US;
        last_update[i]  = now;
    }

    status = RADIO_DISCONNECTED;

    LOG_I("initialization of Radio Controller finish");
};


void radio_read_channels(pwm_pulse_norm_t pulses_us[NUMBER_OF_CHANNELS]) {
    noInterrupts();
    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        pulses_us[i] = pulse_width[i];
    }
    interrupts();

    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        LOG_V("channel %d receiving %d us", i, pulses_us[i]);
    }
};


pwm_pulse_norm_t radio_read_channel(channel_t channel) {
    pwm_pulse_norm_t pulse_us;

    noInterrupts();
    pulse_us = pulse_width[channel];
    interrupts();

    LOG_V("channel %d receiving %d us", channel, pulse_us);

    return pulse_us;
};


radio_status_t radio_status() {
    uint8_t disconnected_channels = 0;

    noInterrupts();
    uint32_t now = micros();
    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        if (now - last_update[i] > RADIO_TIMEOUT_US) {
            disconnected_channels++;
        }
    }
    interrupts();

    static radio_status_t new_status;
    if (disconnected_channels >= NUMBER_OF_CHANNELS - 1) {
        new_status = RADIO_DISCONNECTED;
    } else {
        new_status = RADIO_CONNECTED;
    }

    if (new_status != status) {
        LOG_W("radio status: %d", new_status);

        status = new_status;
    }

    return status;
};


void radio_validate() {
    // TODO
};
