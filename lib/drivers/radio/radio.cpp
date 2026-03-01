#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include "debug.h"
#include "pinout.h"
#include "pwm/pwm.h"
#include "radio.h"
#include "soc/gpio_reg.h"



static volatile uint32_t            rise_time[NUMBER_OF_CHANNELS];
static volatile pwm_pulse_norm_t    pulse_width[NUMBER_OF_CHANNELS];
static volatile uint32_t            last_update[NUMBER_OF_CHANNELS];
static radio_status_t               status = RADIO_DISCONNECTED;

static const uint8_t channels_pins[NUMBER_OF_CHANNELS] = {
    PIN_RADIO_CH1,
    PIN_RADIO_CH2,
    // PIN_RADIO_CH3,
    // PIN_RADIO_CH4,
    // PIN_RADIO_CH5,
    // PIN_RADIO_CH6
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
    uint8_t channel = (uint8_t)(uintptr_t) arg; // portable cast
    uint32_t now    = micros();
    uint32_t level  = (REG_READ(GPIO_IN_REG) >> channels_pins[channel]) & 0x1;

    if (level) {    // rising edge
        rise_time[channel] = now;
    } else {        // falling edge
        pwm_pulse_t pulse_us = (pwm_pulse_t) (now - rise_time[channel]);

        pulse_width[channel] = pwm_pulse_us_normalize(pulse_us);
        last_update[channel] = millis();
    }
}



void radio_init() {
    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization of Radio Controller started");

    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        pinMode(channels_pins[i], INPUT);

        attachInterruptArg(
            digitalPinToInterrupt(channels_pins[i]),
            radio_isr,
            (void*)(uintptr_t) i,
            CHANGE
        );

        rise_time[i]    = 0;
        pulse_width[i]  = (pwm_pulse_norm_t) PWM_NEUTRAL_US;
        last_update[i]  = 0;
    }

    status = RADIO_CONNECTED_DISABLE;

    DEBUG_MSG(DEBUG_LEVEL_INFO, "initialization of Radio Controller finish");
};


void radio_read_channels(pwm_pulse_norm_t pulses_us[NUMBER_OF_CHANNELS]) {
    noInterrupts();
    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        pulses_us[i] = pulse_width[i];
    }
    interrupts();

    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        DEBUG_MSG(
            DEBUG_LEVEL_TRACE,
            "channel %d receiving %d us", i, pulse_width[i]
        );
    }
};


pwm_pulse_norm_t radio_read_channel(channel_t channel) {
    pwm_pulse_norm_t pulse_us;

    noInterrupts();
    pulse_us = pulse_width[channel];
    interrupts();

    DEBUG_MSG(
        DEBUG_LEVEL_TRACE,
        "channel %d receiving %d us", channel, pulse_width[channel]
    );

    return pulse_us;
};


pwm_pulse_t radio_read_pin(uint8_t pin) {
    return pulseIn(pin, HIGH, 25000);
};


radio_status_t radio_status() {
    pwm_pulse_norm_t pulses_us[NUMBER_OF_CHANNELS];
    radio_read_channels(pulses_us);


    bool all_neutral = true;
    for (uint8_t i = 0; i < NUMBER_OF_CHANNELS; i++) {
        if (pulses_us[i] != (pwm_pulse_norm_t) PWM_NEUTRAL_US) {
            all_neutral = false;

            DEBUG_MSG(
                DEBUG_LEVEL_ERROR,
                "channel %d receiving %d us when %d us expected",
                i, pulse_width[i], PWM_NEUTRAL_US
            );

            break;
        }
    }
    if (all_neutral) {
        status = RADIO_DISCONNECTED;

        DEBUG_MSG(
            DEBUG_LEVEL_INFO, "current Radio Controller status is %d", status
        );
        return status;
    }


    // if (pulses_us[CHANNEL_ENABLE] > (pwm_pulse_norm_t) (PWM_NEUTRAL_US + PWM_DEADBAND_US)) {
    //     status = RADIO_CONNECTED_ENABLE;
    // } else {
    //     status = RADIO_CONNECTED_DISABLE;
    // }

    return status;
};


radio_status_t radio_status_pin() {
    pwm_pulse_t steering_us = radio_read_pin(PIN_RADIO_CH1);
    pwm_pulse_t throttle_us = radio_read_pin(PIN_RADIO_CH2);

    if (steering_us == 0 || throttle_us == 0) {
        return RADIO_DISCONNECTED;
    }

    return RADIO_CONNECTED_ENABLE;
};


void radio_validate() {
    // TODO
};
