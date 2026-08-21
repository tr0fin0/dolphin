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
typedef uint8_t pin_t;

/**
 * @def PIN_BUTTON_BOOT
 * @brief Boot Button connection.
 *
 * **Default Value:** `GPIO 00`
 */
#define PIN_BUTTON_BOOT      0

/**
 * @def PIN_ENCODER_L_A
 * @brief Left Motor Encoder A connection.
 *
 * **Default Value:** `GPIO 10`
 */
#define PIN_ENCODER_L_A     10

/**
 * @def PIN_ENCODER_L_B
 * @brief Left Motor Encoder B connection.
 *
 * **Default Value:** `GPIO 21`
 */
#define PIN_ENCODER_L_B     21

/**
 * @def PIN_ENCODER_L_CS
 * @brief Left Motor Encoder CS connection.
 *
 * **Default Value:** `GPIO 48`
 */
#define PIN_ENCODER_L_CS    48

/**
 * @def PIN_ENCODER_R_A
 * @brief Right Motor Encoder A connection.
 *
 * **Default Value:** `GPIO 09`
 */
#define PIN_ENCODER_R_A      9

/**
 * @def PIN_ENCODER_R_B
 * @brief Left Motor Encoder B connection.
 *
 * **Default Value:** `GPIO 14`
 */
#define PIN_ENCODER_R_B     14

/**
 * @def PIN_ENCODER_R_CS
 * @brief Right Motor Encoder CS connection.
 *
 * **Default Value:** `GPIO 47`
 */
#define PIN_ENCODER_R_CS    47

/**
 * @def PIN_ESC_L
 * @brief Left Motor ESC connection.
 *
 * **Default Value:** `GPIO 02`
 */
#define PIN_ESC_L            2

/**
 * @def PIN_ESC_R
 * @brief Right Motor ESC connection.
 *
 * **Default Value:** `GPIO 01`
 */
#define PIN_ESC_R            1

/**
 * @def PIN_I2C_SCL
 * @brief I2C Clock Line connection.
 *
 * **Default Value:** `GPIO 17`
 */
#define PIN_I2C_SCL         17

/**
 * @def PIN_I2C_SDA
 * @brief I2C Data Line connection.
 *
 * **Default Value:** `GPIO 18`
 */
#define PIN_I2C_SDA         18

/**
 * @def PIN_MODULE_START
 * @brief Module Start connection.
 *
 * **Default Value:** `GPIO 35`
 */
#define PIN_MODULE_START    35

/**
 * @def PIN_RADIO_CH1
 * @brief Radio Receiver Channel 1 connection.
 *
 * **Default Value:** `GPIO 42`
 */
#define PIN_RADIO_CH1       42

/**
 * @def PIN_RADIO_CH2
 * @brief Radio Receiver Channel 2 connection.
 *
 * **Default Value:** `GPIO 41`
 */
#define PIN_RADIO_CH2       41

/**
 * @def PIN_RADIO_CH3
 * @brief Radio Receiver Channel 3 connection.
 *
 * **Default Value:** `GPIO 40`
 */
#define PIN_RADIO_CH3       40

/**
 * @def PIN_RADIO_CH4
 * @brief Radio Receiver Channel 4 connection.
 *
 * **Default Value:** `GPIO 39`
 */
#define PIN_RADIO_CH4       39

/**
 * @def PIN_RADIO_CH5
 * @brief Radio Receiver Channel 5 connection.
 *
 * **Default Value:** `GPIO 38`
 */
#define PIN_RADIO_CH5       38

/**
 * @def PIN_RADIO_CH6
 * @brief Radio Receiver Channel 6 connection.
 *
 * **Default Value:** `GPIO 37`
 */
#define PIN_RADIO_CH6       37

/**
 * @def PIN_RGB_LEDS
 * @brief RGB LEDs array initial connection.
 *
 * **Default Value:** `GPIO 08`
 */
#define PIN_RGB_LEDS         8

/**
 * @def PIN_SPARE
 * @brief Unsigned available connection.
 *
 * **Default Value:** `GPIO 36`
 */
#define PIN_SPARE           36

/**
 * @def PIN_SPI_MISO
 * @brief SPI Master In Slave Out connection.
 *
 * **Default Value:** `GPIO 13`
 */
#define PIN_SPI_MISO        13

/**
 * @def PIN_SPI_MOSI
 * @brief SPI Master Out Slave In connection.
 *
 * **Default Value:** `GPIO 11`
 */
#define PIN_SPI_MOSI        11

/**
 * @def PIN_SPI_SCLK
 * @brief SPI Clock connection.
 *
 * **Default Value:** `GPIO 12`
 */
#define PIN_SPI_SCLK        12

/**
 * @def PIN_MUX_VALUE
 * @brief Multiplexer common connection.
 *
 * **Default Value:** `GPIO 07`
 */
#define PIN_MUX_VALUE        7

/**
 * @def PIN_MUX_ENABLE
 * @brief Multiplexer enable connection.
 *
 * **Default Value:** `GPIO 04`
 */
#define PIN_MUX_ENABLE       4

/**
 * @def PIN_MUX_S0
 * @brief Multiplexer selection pin 0 connection.
 *
 * **Default Value:** `GPIO 15`
 */
#define PIN_MUX_S0          15

/**
 * @def PIN_MUX_S1
 * @brief Multiplexer selection pin 1 connection.
 *
 * **Default Value:** `GPIO 16`
 */
#define PIN_MUX_S1          16

/**
 * @def PIN_MUX_S2
 * @brief Multiplexer selection pin 2 connection.
 *
 * **Default Value:** `GPIO 05`
 */
#define PIN_MUX_S2           5

/**
 * @def PIN_MUX_S3
 * @brief Multiplexer selection pin 3 connection.
 *
 * **Default Value:** `GPIO 06`
 */
#define PIN_MUX_S3           6
