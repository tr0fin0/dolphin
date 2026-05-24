/**
 * @file radio.h
 * @brief Capture and normalize PWM pulse widths channels via interruption.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-03
 */

#pragma once

#include <stdint.h>
#include "pwm.h"
#include "pinout.h"

#define RADIO_TIMEOUT_US 25000

/**
 * @brief Radio Controller receiver channels.
 * 
 * Available values are:
 *
 * - `CHANNEL_STEERING`
 *
 * - `CHANNEL_THROTTLE`
 *
 * - `CHANNEL_BUTTON`
 *
 * - `CHANNEL_4`
 *
 * - `CHANNEL_5`
 *
 * - `CHANNEL_6`
 */
typedef enum radio_channel {
    CHANNEL_STEERING = 0,
    CHANNEL_THROTTLE,
    CHANNEL_BUTTON,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    NUMBER_OF_CHANNELS
} radio_channel_t;

/**
 * @brief Radio Controller connection status.
 *
 * Available values are:
 *
 * - `RADIO_CONNECTED`
 *
 * - `RADIO_DISCONNECTED`
 */
typedef enum radio_status {
    RADIO_DISCONNECTED = 0, /**< Radio Controller disconnected after timeout. */
    RADIO_CONNECTED         /**< Radio Controller connected. */
} radio_status_t;

/**
 * @brief Radio Controller PWM pulse width capture via interruptions.
 */
typedef struct radio {
    const char *name;
    radio_status_t status;
    pin_t pins[NUMBER_OF_CHANNELS];
    pwm_norm_t pwms[NUMBER_OF_CHANNELS];
    uint32_t rise_times_us[NUMBER_OF_CHANNELS];
    uint32_t last_times_us[NUMBER_OF_CHANNELS];
} radio_t;

/**
 * @brief Initialize Radio Controller interrupts.
 *
 * @note
 * After initialization, channels start at `PWM_NEUTRAL_US` until pulses are
 * received.
 */
void radio_init(void);

/**
 * @brief Return latest measure pulse width in microseconds from channel.
 *
 * @param channel Radio Controller channel.
 *
 * @return Normalized pulse width in microseconds.
 *
 * @note
 * Interruptions briefly disabled while copying values.
 */
pwm_norm_t radio_read_channel(radio_channel_t channel);

/**
 * @brief Return latest measured pulse widths in microseconds from all channels.
 *
 * @param pwms Array of Radio Controller channels.
 *
 * @note
 * Interruptions briefly disabled while copying values.
 */
void radio_read_channels(pwm_norm_t pwms[NUMBER_OF_CHANNELS]);

/**
 * @brief Returns latest Radio Controller status.
 */
radio_status_t radio_status(void);
