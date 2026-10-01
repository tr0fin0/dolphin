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
 * @brief Radio channels.
 */
typedef enum radio_channel {
    RADIO_CHANNEL_1 = 0,        /**< Radio channel 1. */
    RADIO_CHANNEL_2,            /**< Radio channel 2. */
    RADIO_CHANNEL_3,            /**< Radio channel 3. */
    RADIO_CHANNEL_4,            /**< Radio channel 4. */
    RADIO_CHANNEL_5,            /**< Radio channel 5. */
    RADIO_CHANNEL_6,            /**< Radio channel 6. */
    NUMBER_OF_RADIO_CHANNELS    /**< Number of radio channels. */
} radio_channel_t;

/**
 * @brief Radio connection status.
 */
typedef enum radio_status {
    RADIO_STATUS_DISCONNECTED = 0,  /**< All channels disconnected after timeout. */
    RADIO_STATUS_CONNECTED,         /**< All channels receiving. */
    NUMBER_OF_RADIO_STATUS          /**< Number of radio status. */
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
 * @brief Radio disconnection timeout interval in microseconds.
 *
 * **Default Value:** 25 us
 */
#define RADIO_TIMEOUT_US 25000

/**
 * @brief Returns the current radio channel name.
 *
 * @return Human-readable null-terminated string representing the channel name.
 */
const char *radio_get_channel_name(radio_channel_t channel);

/**
 * @brief Returns the current radio name.
 *
 * @return Human-readable null-terminated string representing the radio name.
 */
const char *radio_get_name(void);

/**
 * @brief Returns the latest radio status.
 *
 * @return Current connection status
 */
radio_status_t radio_get_status(void);

/**
 * @brief Returns the current radio status name.
 *
 * @return Human-readable null-terminated string representing the status name.
 */
const char *radio_get_status_name(void);

/**
 * @brief Initialize radio interrupts.
 *
 * @note After initialization, channels start at @ref PWM_NEUTRAL_US until
 * pulses are received.
 */
void radio_init(void);

/**
 * @brief Return latest measured pulse width in microseconds from channel.
 *
 * @note Interruptions briefly disabled while copying values.
 *
 * @param[in] channel Radio channel.
 *
 * @return Normalized pulse width in microseconds.
 */
pwm_norm_t radio_read_channel(radio_channel_t channel);

/**
 * @brief Return latest measured pulse widths in microseconds from all channels.
 *
 * @note Interruptions briefly disabled while copying values.
 *
 * @param[in] pwms Array of radio channels.
 *
 * @return Normalized array of pulses width in microseconds.
 */
void radio_read_channels(pwm_norm_t *pwms);
