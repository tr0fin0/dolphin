/**
 * @file mux.h
 * @brief Multiplexer abstraction.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-02
 */

#pragma once

#include "esp_adc/adc_oneshot.h"
#include "esp_timer.h"
#include "pinout.h"

/**
 * @brief Multiplexer address pins.
 *
 * @note Each @ref mux_channel is accessed via a binary address combination.
 */
typedef enum mux_address {
    MUX_ADDRESS_0 = 0,          /**< Multiplexer address ``0`` pin. */
    MUX_ADDRESS_1,              /**< Multiplexer address ``1`` pin. */
    MUX_ADDRESS_2,              /**< Multiplexer address ``2`` pin. */
    MUX_ADDRESS_3,              /**< Multiplexer address ``3`` pin. */
    NUMBER_OF_MUX_ADDRESSES,    /**< Number of multiplexer address pins. */
} mux_address_t;

/**
 * @brief Multiplexer channels pins.
 *
 * @note Each @ref mux_address represent a bit in the channel address.
 */
typedef enum mux_channel {
    MUX_CHANNEL_00 = 0,     /**< Multiplexer channel ``00`` address ``0b0000``. */
    MUX_CHANNEL_01,         /**< Multiplexer channel ``01`` address ``0b0001``. */
    MUX_CHANNEL_02,         /**< Multiplexer channel ``02`` address ``0b0010``. */
    MUX_CHANNEL_03,         /**< Multiplexer channel ``03`` address ``0b0011``. */
    MUX_CHANNEL_04,         /**< Multiplexer channel ``04`` address ``0b0100``. */
    MUX_CHANNEL_05,         /**< Multiplexer channel ``05`` address ``0b0101``. */
    MUX_CHANNEL_06,         /**< Multiplexer channel ``06`` address ``0b0110``. */
    MUX_CHANNEL_07,         /**< Multiplexer channel ``07`` address ``0b0111``. */
    MUX_CHANNEL_08,         /**< Multiplexer channel ``08`` address ``0b1000``. */
    MUX_CHANNEL_09,         /**< Multiplexer channel ``09`` address ``0b1001``. */
    MUX_CHANNEL_10,         /**< Multiplexer channel ``10`` address ``0b1010``. */
    MUX_CHANNEL_11,         /**< Multiplexer channel ``11`` address ``0b1011``. */
    MUX_CHANNEL_12,         /**< Multiplexer channel ``12`` address ``0b1100``. */
    MUX_CHANNEL_13,         /**< Multiplexer channel ``13`` address ``0b1101``. */
    MUX_CHANNEL_14,         /**< Multiplexer channel ``14`` address ``0b1110``. */
    MUX_CHANNEL_15,         /**< Multiplexer channel ``15`` address ``0b1111``. */
    NUMBER_OF_MUX_CHANNELS, /**< Number of multiplexer channels pins. */
} mux_channel_t;

/**
 * @def MUX_BUFFER_SIZE
 * @brief Multiplexer measurements buffer size.
 *
 * **Default Value:** ``5``
 */
#define MUX_BUFFER_SIZE 5

/**
 * @brief Multiplexer circular buffer.
 */
typedef struct mux_buffer {
    bool active;                        /**< When active is ``true``, ADC buffer measurements are stored. */
    uint16_t measures[MUX_BUFFER_SIZE]; /**< ADC buffer measurements. */
    uint8_t head;                       /**< Position of the oldest value in the buffer. */
} mux_buffer_t;

/**
 * @brief Multiplexer abstraction.
 */
typedef struct mux {
    const char *name;                               /**< Human-readable null-terminated Multiplexer name. */
    adc_oneshot_unit_handle_t adc_handle;           /**< Multiplexer ADC unit handler. */
    esp_timer_handle_t timer_handle;                /**< Multiplexer Timer handler. */
    mux_buffer_t buffers[NUMBER_OF_MUX_CHANNELS];   /**< Multiplexer ADC measurements buffer. */
    mux_channel_t current_channel;                  /**< Multiplexer current measured channel. */
    pin_t address[NUMBER_OF_MUX_ADDRESSES];         /**< Multiplexer address pins. */
    pin_t common;                                   /**< Multiplexer common pin. */
#if defined(CONFIG_MAINBOARD_V1)
    pin_t enable;                                   /**< Multiplexer enable pin. */
#endif
} mux_t;

/**
 * @def MUX_ADC_ATTENUATION
 * @brief Multiplexer ADC attenuation in decibels.
 *
 * **Default Value:** ``ADC_ATTEN_DB_12``
 */
#define MUX_ADC_ATTENUATION ADC_ATTEN_DB_12

/**
 * @def MUX_ADC_BITWIDTH
 * @brief Multiplexer ADC bitwidth in bits.
 *
 * **Default Value:** ``ADC_BITWIDTH_12``
 *
 * @note ESP32S3 has a 12 bit resolution ADC.
 */
#define MUX_ADC_BITWIDTH    ADC_BITWIDTH_12

#if defined(CONFIG_MAINBOARD_V2)
/**
 * @def MUX_ADC_CHANNEL
 * @brief Multiplexer ADC channel.
 *
 * **Default Value:** ``ADC_CHANNEL_5``
 *
 * @note Depending on the @ref PIN_MUX_COMMON , a different ADC channel is
 * available.
 */
#define MUX_ADC_CHANNEL     ADC_CHANNEL_5
#elif defined(CONFIG_MAINBOARD_V1)
/**
 * @def MUX_ADC_CHANNEL
 * @brief Multiplexer ADC channel.
 *
 * **Default Value:** ``ADC_CHANNEL_6``
 *
 * @note Depending on the @ref PIN_MUX_COMMON , a different ADC channel is
 * available.
 */
#define MUX_ADC_CHANNEL     ADC_CHANNEL_6
#endif

/**
 * @def MUX_ADC_RESOLUTION
 * @brief Multiplexer ADC resolution.
 *
 * **Default Value:** ``4095.0f``
 *
 * @note Based in the @ref MUX_ADC_BITWIDTH value: \f[ 4095 = 2^{12} - 1 \f]
 */
#define MUX_ADC_RESOLUTION  4095.0f

#if defined(CONFIG_MAINBOARD_V2)
/**
 * @def MUX_ADC_UNIT
 * @brief Multiplexer ADC unit.
 *
 * **Default Value:** ``ADC_UNIT_2``
 *
 * @note Depending on the @ref PIN_MUX_COMMON , a different ADC unit is
 * available.
 *
 * @warning ADC2 is shared with the Wi-Fi. Therefore, only one may be used at a
 * time.
 */
#define MUX_ADC_UNIT        ADC_UNIT_2
#elif defined(CONFIG_MAINBOARD_V1)
/**
 * @def MUX_ADC_UNIT
 * @brief Multiplexer ADC unit.
 *
 * **Default Value:** ``ADC_UNIT_1``
 *
 * @note Depending on the @ref PIN_MUX_COMMON , a different ADC unit is
 * available.
 */
#define MUX_ADC_UNIT        ADC_UNIT_1
#endif

/**
 * @def MUX_TIMER_PERIOD_US
 * @brief Multiplexer Timer Period in microseconds.
 *
 * @note Ensure timer period is set to at least 3 to 4 times the duration of the
 * logging execution time.
 *
 * **Default Value:** ``1000 us``
 */
#define MUX_TIMER_PERIOD_US 1000

/**
 * @brief Return the Multiplexer name.
 *
 * @return Human-readable null-terminated string representing the multiplexer
 * name.
 */
const char *mux_get_name(void);

/**
 * @brief Initialization of the Multiplexer.
 *
 * @param[in] buffers_config Array of multiplexer channel buffer configurations.
 */
void mux_init(mux_buffer_t *buffers_config);

/**
 * @brief Return the Multiplexer channel value as percentage of @ref MUX_ADC_RESOLUTION.
 *
 * The measured sensor value range between ``[0.0000, 1.0000]`` representing a
 * percentage of the maximum 3V3 voltage supported.
 *
 * @param[in] channel Multiplexer channel.
 *
 * @return Measured multiplexer channel value.
 */
float mux_read_channel(mux_channel_t channel);
