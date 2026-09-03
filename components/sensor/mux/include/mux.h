/**
 * @file mux.h
 * @brief Multiplexer abstraction.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-02
 */

#pragma once

#include "esp_adc/adc_oneshot.h"
#include "pinout.h"

/**
 * @brief Multiplexer address pins.
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
 */
typedef enum mux_channel {
    MUX_CHANNEL_00 = 0,     /**< Multiplexer channel ``00``. */
    MUX_CHANNEL_01,         /**< Multiplexer channel ``01``. */
    MUX_CHANNEL_02,         /**< Multiplexer channel ``02``. */
    MUX_CHANNEL_03,         /**< Multiplexer channel ``03``. */
    MUX_CHANNEL_04,         /**< Multiplexer channel ``04``. */
    MUX_CHANNEL_05,         /**< Multiplexer channel ``05``. */
    MUX_CHANNEL_06,         /**< Multiplexer channel ``06``. */
    MUX_CHANNEL_07,         /**< Multiplexer channel ``07``. */
    MUX_CHANNEL_08,         /**< Multiplexer channel ``08``. */
    MUX_CHANNEL_09,         /**< Multiplexer channel ``09``. */
    MUX_CHANNEL_10,         /**< Multiplexer channel ``10``. */
    MUX_CHANNEL_11,         /**< Multiplexer channel ``11``. */
    MUX_CHANNEL_12,         /**< Multiplexer channel ``12``. */
    MUX_CHANNEL_13,         /**< Multiplexer channel ``13``. */
    MUX_CHANNEL_14,         /**< Multiplexer channel ``14``. */
    MUX_CHANNEL_15,         /**< Multiplexer channel ``15``. */
    NUMBER_OF_MUX_CHANNELS, /**< Number of multiplexer channels pins. */
} mux_channel_t;

/**
 * @brief Multiplexer abstraction.
 */
typedef struct mux {
    const char *name;                       /**< Human-readable null-terminated Multiplexer name. */
    adc_oneshot_unit_handle_t adc_handle;   /**< Multiplexer ADC unit handler. */
    pin_t address[NUMBER_OF_MUX_ADDRESSES]; /**< Multiplexer address pins. */
    pin_t common;                           /**< Multiplexer common pin. */
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
 * **Default Value:** ``ADC_BITWIDTH_DEFAULT``
 */
#define MUX_ADC_BITWIDTH    ADC_BITWIDTH_DEFAULT

/**
 * @def MUX_ADC_CHANNEL
 * @brief Multiplexer ADC channel.
 *
 * **Default Value:** ``ADC_CHANNEL_5``
 */
#define MUX_ADC_CHANNEL     ADC_CHANNEL_5

/**
 * @def MUX_ADC_RESOLUTION
 * @brief Multiplexer ADC resolution.
 *
 * **Default Value:** 4095
 */
#define MUX_ADC_RESOLUTION  4095

/**
 * @def MUX_ADC_UNIT
 * @brief Multiplexer ADC unit.
 *
 * **Default Value:** ``ADC_UNIT_2``
 */
#define MUX_ADC_UNIT        ADC_UNIT_2

/**
 * @brief Return the Multiplexer name.
 *
 * @return Human-readable null-terminated string representing the multiplexer
 * name.
 */
const char *mux_get_name(void);

/**
 * @brief Initialization of the Multiplexer.
 */
void mux_init(void);

/**
 * @brief Return the Multiplexer channel value as percentage of @ref MUX_ADC_RESOLUTION.
 *
 * The measured sensor value range between ``[0.0000, 1.0000]`` representing a
 * percentage of the maximum 3V3 voltage supported.
 *
 * @param[in] channel Multiplexer channel.
 * @return Measured Multiplexer channel value.
 */
float mux_read_channel(mux_channel_t channel);
