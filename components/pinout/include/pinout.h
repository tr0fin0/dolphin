/**
 * @file pinout.h
 * @brief Mainboard pinout.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-04-26
 */

#pragma once

#include <stdint.h>

/**
 * @brief Physical connection on the mainboard board.
 */
typedef enum pin {
    PIN_ENC_L = 47,         /**< ``GPIO 47`` : @ref ENCODER_L connection. */
    PIN_ENC_R = 9,          /**< ``GPIO 09`` : @ref ENCODER_R connection. */
    PIN_ESC_L = 48,         /**< ``GPIO 48`` : @ref ESC_L connection. */
    PIN_ESC_R = 8,          /**< ``GPIO 08`` : @ref ESC_R connection. */
    PIN_I2C_SCL = 17,       /**< ``GPIO 17`` : I2C clock line connection. */
    PIN_I2C_SDA = 18,       /**< ``GPIO 18`` : I2C data line connection. */
    PIN_IO00 = 0,           /**< ``GPIO 00`` : actively low, enables ESP32 boot mode. */
    PIN_IO10 = 10,          /**< ``GPIO 10`` : spare connection. */
    PIN_IO21 = 21,          /**< ``GPIO 21`` : spare connection. */
    PIN_IO35 = 35,          /**< ``GPIO 35`` : spare connection. */
    PIN_IO36 = 36,          /**< ``GPIO 36`` : spare connection. */
    PIN_IO37 = 37,          /**< ``GPIO 37`` : spare connection. */
    PIN_IRR = 1,            /**< ``GPIO 01`` : infrared receiver connection. */
    PIN_LRGB = 4,           /**< ``GPIO 04`` : @ref led_array_t pin connection. */
    PIN_MUX_COMMON = 16,    /**< ``GPIO 16`` : @ref mux_t common pin connection. */
    PIN_MUX_ADDR0 = 7,      /**< ``GPIO 07`` : @ref mux_t address pin 0 connection. */
    PIN_MUX_ADDR1 = 15,     /**< ``GPIO 15`` : @ref mux_t address pin 1 connection. */
    PIN_MUX_ADDR2 = 5,      /**< ``GPIO 05`` : @ref mux_t address pin 2 connection. */
    PIN_MUX_ADDR3 = 6,      /**< ``GPIO 06`` : @ref mux_t address pin 3 connection. */
    PIN_RC_CH1 = 2,         /**< ``GPIO 02`` : @ref RADIO_CHANNEL_1 connection. */
    PIN_RC_CH2 = 42,        /**< ``GPIO 42`` : @ref RADIO_CHANNEL_2 connection. */
    PIN_RC_CH3 = 41,        /**< ``GPIO 41`` : @ref RADIO_CHANNEL_3 connection. */
    PIN_RC_CH4 = 40,        /**< ``GPIO 40`` : @ref RADIO_CHANNEL_4 connection. */
    PIN_RC_CH5 = 39,        /**< ``GPIO 39`` : @ref RADIO_CHANNEL_5 connection. */
    PIN_RC_CH6 = 38,        /**< ``GPIO 38`` : @ref RADIO_CHANNEL_6 connection. */
    PIN_SPI_MISO = 13,      /**< ``GPIO 13`` : SPI Master In Slave Out connection. */
    PIN_SPI_MOSI = 11,      /**< ``GPIO 11`` : SPI Master Out Slave In connection. */
    PIN_SPI_SCLK = 12,      /**< ``GPIO 12`` : SPI clock connection. */
    NUMBER_OF_PINS = 28     /**< Number of pin connections. */
} pin_t;
