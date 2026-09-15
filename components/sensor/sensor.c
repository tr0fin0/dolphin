#include "sensor.h"

static sensor_config_t sensors[NUMBER_OF_SENSORS] = {
    [SENSOR_JS2_LR] = {
        .name       = "JS200XF_LATERAL_R",
        .channel    = MUX_CHANNEL_03,
        .mode       = SENSOR_MODE_DIGITAL,
        .threshold  = 0.500f
    },
    [SENSOR_JS2_DR] = {
        .name       = "JS200XF_DIAGONAL_R",
        .channel    = MUX_CHANNEL_02,
        .mode       = SENSOR_MODE_DIGITAL,
        .threshold  = 0.500f
    },
    [SENSOR_JS2_FR] = {
        .name       = "JS200XF_FRONT_R",
        .channel    = MUX_CHANNEL_01,
        .mode       = SENSOR_MODE_DIGITAL,
        .threshold  = 0.500f
    },
    [SENSOR_JS2_FL] = {
        .name       = "JS200XF_FRONT_L",
        .channel    = MUX_CHANNEL_14,
        .mode       = SENSOR_MODE_DIGITAL,
        .threshold  = 0.500f
    },
    [SENSOR_JS2_DL] = {
        .name       = "JS200XF_DIAGONAL_L",
        .channel    = MUX_CHANNEL_13,
        .mode       = SENSOR_MODE_DIGITAL,
        .threshold  = 0.500f
    },
    [SENSOR_JS2_LL] = {
        .name       = "JS200XF_LATERAL_L",
        .channel    = MUX_CHANNEL_12,
        .mode       = SENSOR_MODE_DIGITAL,
        .threshold  = 0.500f
    },
    [SENSOR_QRE_BR] = {
        .name       = "QRE1113_BACK_R",
        .channel    = MUX_CHANNEL_05,
        .mode       = SENSOR_MODE_ANALOG,
    },
    [SENSOR_QRE_FR] = {
        .name       = "QRE1113_FRONT_R",
        .channel    = MUX_CHANNEL_04,
        .mode       = SENSOR_MODE_ANALOG,
    },
    [SENSOR_QRE_BL] = {
        .name       = "QRE1113_BACK_L",
        .channel    = MUX_CHANNEL_10,
        .mode       = SENSOR_MODE_ANALOG,
    },
    [SENSOR_QRE_FL] = {
        .name       = "QRE1113_FRONT_L",
        .channel    = MUX_CHANNEL_11,
        .mode       = SENSOR_MODE_ANALOG,
    },
};

const char *sensor_get_name(sensor_t sensor) {
    return sensors[sensor].name;
}

float sensor_get_value(sensor_t sensor) {
    float sensor_value = mux_read_channel(sensors[sensor].channel);

    if(sensors[sensor].mode == SENSOR_MODE_DIGITAL) {
        if (sensor_value >= sensors[sensor].threshold) {
            return SENSOR_MAX_VALUE;
        } else {
            return SENSOR_MIN_VALUE;
        };
    };

    if (sensor_value > SENSOR_MAX_VALUE) sensor_value = SENSOR_MAX_VALUE;
    if (sensor_value < SENSOR_MIN_VALUE) sensor_value = SENSOR_MIN_VALUE;

    return sensor_value;
}

void sensor_init(void) {
    mux_init();
}
