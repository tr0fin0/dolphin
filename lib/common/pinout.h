#ifndef __PINOUT_H__
#define __PINOUT_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



// COMMUNICATION
#define PIN_I2C_SCL         17
#define PIN_I2C_SDA         18

#define PIN_MODULE_START    35

#define PIN_RADIO_CH1       42
#define PIN_RADIO_CH2       41
#define PIN_RADIO_CH3       40
#define PIN_RADIO_CH4       39
#define PIN_RADIO_CH5       38
#define PIN_RADIO_CH6       37


// DEBUG
#define PIN_BUTTON_BOOT      0

#define PIN_RGB_LEDS         8

#define PIN_SPARE           36


// ENCODERS
#define PIN_SPI_MOSI        11
#define PIN_SPI_SCLK        12
#define PIN_SPI_MISO        13

#define PIN_ENCODER_L_A     10
#define PIN_ENCODER_L_B     21
#define PIN_ENCODER_L_CS    48
#define PIN_ENCODER_R_A      9
#define PIN_ENCODER_R_B     14
#define PIN_ENCODER_R_CS    47


// SENSORS
#define PIN_MUX_VALUE        7
#define PIN_MUX_ENABLE       4
#define PIN_MUX_S0          15
#define PIN_MUX_S1          16
#define PIN_MUX_S2           5
#define PIN_MUX_S3           6


// MOTORS
#define PIN_ESC_L            2
#define PIN_ESC_R            1



#ifdef __cplusplus
}
#endif

#endif /* __PINOUT_H__ */
