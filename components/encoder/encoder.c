#include "esp_err.h"
#include "encoder.h"
#include "logging.h"

static encoder_config_t encoders[NUMBER_OF_ENCODERS] = {
    [ENCODER_L] = {
        .name = "ENC_L",
        .clock_hz = AS5048A_FREQUENCY_HZ,
        .pin_cs = PIN_ENCODER_L_CS,
        .spi_mode = SPI_MODE_1},
    [ENCODER_R] = {.name = "ENC_R", .clock_hz = AS5048A_FREQUENCY_HZ, .pin_cs = PIN_ENCODER_R_CS, .spi_mode = SPI_MODE_1}};

static uint16_t swap_endianess_16(uint16_t value)
{
    return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
};

void encoder_init(void)
{
    esp_err_t ret;

    spi_bus_config_t bus_config = {
        .miso_io_num = PIN_SPI_MISO,
        .mosi_io_num = PIN_SPI_MOSI,
        .sclk_io_num = PIN_SPI_SCLK,
        .quadwp_io_num = -1, // not used
        .quadhd_io_num = -1, // not used
        .max_transfer_sz = 32};

    ret = spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_DISABLED);
    if (ret != ESP_OK)
    {
        LOG_E(
            "SPI bus initialization failed with error %s.", esp_err_to_name(ret));

        return;
    }

    for (encoder_t i = 0; i < NUMBER_OF_ENCODERS; i++)
    {
        spi_device_interface_config_t encoder_config = {
            .clock_speed_hz = encoders[i].clock_hz,
            .mode = encoders[i].spi_mode,
            .spics_io_num = encoders[i].pin_cs,
            .queue_size = 1,
            .flags = SPI_DEVICE_HALFDUPLEX};

        ret = spi_bus_add_device(
            SPI2_HOST, &encoder_config, &encoders[i].spi_handle);
        if (ret != ESP_OK)
        {
            LOG_E(
                "%s SPI bus addition failed with error: %s.",
                encoders[i].name, esp_err_to_name(ret));

            return;
        }

        encoder_read_angle(i);

        LOG_I(
            "%s initialized on pin %d.", encoders[i].name, encoders[i].pin_cs);
    }

    return;
}

const char *encoder_get_name(encoder_t encoder)
{
    return encoders[encoder].name;
}

angle_t encoder_read_angle(encoder_t encoder)
{
    spi_transaction_t t = {
        .length = 16,
        .flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA,
    };

    uint16_t command = swap_endianess_16(AS5048A_CMD_ANGLE);
    t.tx_data[0] = (uint8_t)((command & 0x00FF));
    t.tx_data[1] = (uint8_t)((command & 0xFF00) >> 8);

    esp_err_t ret = spi_device_polling_transmit(
        encoders[encoder].spi_handle, &t);
    if (ret != ESP_OK)
    {
        LOG_E(
            "%s SPI transmittion failed with error: %s.",
            encoders[encoder].name, esp_err_to_name(ret));

        return 0;
    }

    angle_t angle = (t.rx_data[0] << 8) | t.rx_data[1];

    return angle & AS5048A_REG_DATA;
}
