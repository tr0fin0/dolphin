/**
 * @file encoder.h
 * @brief SPI communication library for the magnetic rotary encoder AS5048A.
 *
 * For more information:
 *
 * - https://look.ams-osram.com/m/287d7ad97d1ca22e/original/AS5048-DS000298.pdf
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-04-26
 */

#pragma once

#include <stdint.h>
#include "driver/spi_master.h"
#include "pinout.h"

#define AS5048A_REG_PAR     (1 << 15)
#define AS5048A_REG_RWN     (1 << 14)
#define AS5048A_REG_DATA    0x3FFF
#define AS5048A_CMD_NOP     0x0000
#define AS5048A_CMD_ANGLE   (AS5048A_REG_PAR || AS5048A_REG_RWN || AS5048A_REG_DATA)
#define AS5048A_FREQUENCY_HZ 10000000 /** 10 MHz */
/**
 * @brief Angle in degrees.
 */
typedef uint16_t angle_t;

/**
 * @brief SPI combinations of polarity and phase.
 *
 * Each mode is obtained with a combination of the variables `CPOL` and `CPHA`:
 *
 * - `CPOL = 0` and `CPHA = 0`
 *
 * - `CPOL = 0` and `CPHA = 1`
 *
 * - `CPOL = 1` and `CPHA = 0`
 *
 * - `CPOL = 1` and `CPHA = 1`
 *
 */
typedef enum {
    /** Data shifted out on failing SCLK when SS activates with data sampled on rising SCLK */
    SPI_MODE_0 = 0,
    /** Data shifted out on rising SCLK with data sampled on failing SCLK */
    SPI_MODE_1,
    /** Data shifted out on rising SCLK when SS activates with data sampled on failing SCLK */
    SPI_MODE_2,
    /** Data shifted out on failing SCLK with data sampled on rising SCLK */
    SPI_MODE_3,
} encoder_spi_mode_t;

/**
 * @brief Encoder configuration.
 */
typedef struct {
    const char *name;
    uint32_t clock_hz;
    pin_t pin_cs;
    spi_device_handle_t spi_handle;
    encoder_spi_mode_t spi_mode;
} encoder_config_t;

/**
 * @brief Encoders on the project.
 *
 * Available values are:
 *
 * - `ENCODER_L`
 *
 * - `ENCODER_R`
 */
typedef enum {
    ENCODER_L = 0,      /** Encoder positioned on the left motor*/
    ENCODER_R,          /** Encoder positioned on the right motor */
    NUMBER_OF_ENCODERS
} encoder_t;

/**
 * @brief Initialization all encoders to the same SPI bus.
 */
void encoder_init(void);

/**
 * @brief Returns encoder's name.
 *
 * @param encoder Encoder on the project.
 *
 * @return Encoder's name.
 */
const char *encoder_get_name(encoder_t encoder);

/**
 * @brief Returns encoder's measured angle.
 *
 * @param encoder Encoder on the project.
 *
 * @return Current measure angle for encoder.
 */
angle_t encoder_read_angle(encoder_t encoder);
