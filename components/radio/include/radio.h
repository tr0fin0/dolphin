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

/**
 * @brief Radio Receiver receiver channels.
 *
 * Available values are:
 *
 * - `RADIO_CHANNEL_STEERING`
 *
 * - `RADIO_CHANNEL_THROTTLE`
 *
 * - `RADIO_CHANNEL_BUTTON`
 *
 * - `RADIO_CHANNEL_4`
 *
 * - `RADIO_CHANNEL_5`
 *
 * - `RADIO_CHANNEL_6`
 */
typedef enum radio_channel {
    RADIO_CHANNEL_STEERING = 0,
    RADIO_CHANNEL_THROTTLE,
    RADIO_CHANNEL_BUTTON,
    RADIO_CHANNEL_4,
    RADIO_CHANNEL_5,
    RADIO_CHANNEL_6,
    NUMBER_OF_RADIO_CHANNELS
} radio_channel_t;

/**
 * @brief Radio Receiver connection status.
 *
 * Available values are:
 *
 * - `RADIO_CONNECTED`
 *
 * - `RADIO_DISCONNECTED`
 */
typedef enum radio_status {
    RADIO_DISCONNECTED = 0, /**< Radio Receiver disconnected after timeout. */
    RADIO_CONNECTED,        /**< Radio Receiver connected. */
    NUMBER_OF_RADIO_STATUS
} radio_status_t;

/**
 * @brief Radio Receiver PWM pulse width capture via interruptions.
 */
typedef struct radio {
    const char *name;
    radio_status_t status;
    const char *status_names[NUMBER_OF_RADIO_STATUS];
    pin_t pins[NUMBER_OF_RADIO_CHANNELS];
    pwm_norm_t pwms[NUMBER_OF_RADIO_CHANNELS];
    int64_t rise_times_us[NUMBER_OF_RADIO_CHANNELS];
    int64_t last_times_us[NUMBER_OF_RADIO_CHANNELS];
} radio_t;

#define RADIO_TIMEOUT_US 25000

/**
 * @brief Initialize Radio Receiver interrupts.
 *
 * @note
 * After initialization, channels start at `PWM_NEUTRAL_US` until pulses are
 * received.
 */
void radio_init(void);

/**
 * @brief Return latest measured pulse width in microseconds from channel.
 *
 * @param channel Radio Receiver channel.
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
 * @param pwms Array of Radio Receiver channels.
 *
 * @note
 * Interruptions briefly disabled while copying values.
 */
void radio_read_channels(pwm_norm_t pwms[NUMBER_OF_RADIO_CHANNELS]);

/**
 * @brief Returns the Radio Receiver status name as a null-terminated string.
 */
const char *radio_status_name();

/**
 * @brief Returns latest Radio Receiver status.
 */
radio_status_t radio_status(void);
