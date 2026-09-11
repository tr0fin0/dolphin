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
 */
typedef enum radio_channel {
    RADIO_CHANNEL_1 = 0,        /**< Radio Receiver channel 1. */
    RADIO_CHANNEL_2,            /**< Radio Receiver channel 2. */
    RADIO_CHANNEL_3,            /**< Radio Receiver channel 3. */
    RADIO_CHANNEL_4,            /**< Radio Receiver channel 4. */
    RADIO_CHANNEL_5,            /**< Radio Receiver channel 5. */
    RADIO_CHANNEL_6,            /**< Radio Receiver channel 6. */
    NUMBER_OF_RADIO_CHANNELS    /**< Number of Radio Channels. */
} radio_channel_t;

/**
 * @brief Radio Receiver connection status.
 */
typedef enum radio_status {
    RADIO_DISCONNECTED = 0, /**< Radio Receiver disconnected after timeout. */
    RADIO_CONNECTED,        /**< Radio Receiver connected. */
    NUMBER_OF_RADIO_STATUS  /**< Number of Radio Status. */
} radio_status_t;

/**
 * @brief Radio Receiver connection.
 */
typedef struct radio_connection {
    const char *name;       /**< Human-readable null-terminated connection name. */
    pin_t pin;              /**< Radio Connection channel pin. */
    pwm_norm_t pwm;         /**< Radio Connection channel latest normalized PWM pulse width. */
    int64_t rise_time_us;   /**< Radio Connection last rising time in microseconds. */
    int64_t last_time_us;   /**< Radio Connection last update time in microseconds. */
} radio_connection_t;

/**
 * @brief Radio Receiver PWM pulse width capture via interruptions.
 */
typedef struct radio {
    const char *name;                                           /**< Human-readable null-terminated Radio name. */
    radio_status_t status;                                      /**< Radio Receiver current connection status. */
    const char *status_names[NUMBER_OF_RADIO_STATUS];           /**< Human-readable null-terminated Radio status. */
    radio_connection_t connections[NUMBER_OF_RADIO_CHANNELS];   /**< Radio Connections. */
} radio_t;

/**
 * @def RADIO_TIMEOUT_US
 * @brief Radio Receiver disconnection timeout interval in microseconds.
 *
 * **Default Value:** 25 us
 */
#define RADIO_TIMEOUT_US 25000

/**
 * @brief Returns the current Radio Receiver connection name.
 *
 * @return Human-readable null-terminated string representing the name.
 */
const char *radio_get_channel_name(radio_channel_t channel);

/**
 * @brief Returns the current Radio Receiver name.
 *
 * @return Human-readable null-terminated string representing the name.
 */
const char *radio_get_name(void);

/**
 * @brief Returns the latest Radio Receiver status.
 *
 * @return Current connection status
 */
radio_status_t radio_get_status(void);

/**
 * @brief Returns the current Radio Receiver status name.
 *
 * @return Human-readable null-terminated string representing the status.
 */
const char *radio_get_status_name(void);

/**
 * @brief Initialize Radio Receiver interrupts.
 *
 * @note
 * After initialization, channels start at @ref PWM_NEUTRAL_US until pulses are
 * received.
 */
void radio_init(void);

/**
 * @brief Return latest measured pulse width in microseconds from channel.
 *
 * @note Interruptions briefly disabled while copying values.
 *
 * @param[in] channel Radio Receiver channel.
 * @return Normalized pulse width in microseconds.
 */
pwm_norm_t radio_read_channel(radio_channel_t channel);

/**
 * @brief Return latest measured pulse widths in microseconds from all channels.
 *
 * @note Interruptions briefly disabled while copying values.
 *
 * @param[in] pwms Array of Radio Receiver channels.
 * @return Normalized array of pulses width in microseconds.
 */
void radio_read_channels(pwm_norm_t *pwms);
