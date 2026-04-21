/**
 * @file radio.h
 * @brief Capture and normalize RC pulse-width channels (interrupt-driven).
 *
 * - Implements input capture for NUMBER_OF_CHANNELS PWM channels.
 * - ISR stores normalized pulse widths in microseconds.
 */

#ifndef __RADIO_H__
#define __RADIO_H__

#include <stdint.h>
#include "pwm.h"



#ifdef __cplusplus
extern "C" {
#endif



#define RADIO_TIMEOUT_US    20000


/**
 * @brief Logical indices for RC receiver channels.
 */
typedef enum {
    CHANNEL_STEERING = 0,
    CHANNEL_THROTTLE,
    CHANNEL_BUTTON,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    NUMBER_OF_CHANNELS
} channel_t;

/**
 * @brief Radio connection and authorization status.
 *
 * Represents the current operational state of the RC link:
 *
 * - `RADIO_CONNECTED`:     Valid signal present and enable switch active.
 *
 * - `RADIO_DISCONNECTED`:  No valid signal detected.
 */
typedef enum {
    RADIO_CONNECTED,    /**< Valid signal present and enable switch active. */
    RADIO_DISCONNECTED  /**< No valid signal detected. */
} radio_status_t;



/**
 * @brief Initialize hardware driver for Radio Controller interrupts.
 *
 * After initialization, channels start at `PWM_NEUTRAL_US` until pulses are
 * received.
 */
void radio_init(void);

/**
 * @brief Copy latest measured pulse widths in microseconds into buffer.
 *
 * @param pulses_us Array of size `NUMBER_OF_CHANNELS` to receive values.
 *
 * This call disables interrupts briefly while copying to ensure consistency.
 */
void radio_read_channels(pwm_pulse_norm_t pulses_us[NUMBER_OF_CHANNELS]);

/**
 * @brief Read a single channel pulse width in microseconds.
 *
 * @return Normalized pulse width in microseconds.
 */
pwm_pulse_norm_t radio_read_channel(channel_t channel);

/**
 * @brief Returns the current radio connection state.
 */
radio_status_t radio_status(void);


void radio_validate(void);



#ifdef __cplusplus
}
#endif

#endif /* __RADIO_H__ */
