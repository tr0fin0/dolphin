/**
 * @file sensor.h
 * @brief Analogic and Digital sensor abstraction from MUX connections.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-02
 */

#pragma once

#include "mux.h"
#include <stdbool.h>

/**
 * @brief Sensor positions.
 */
typedef enum sensor {
    SENSOR_JS2_DL = 0,  /**< Obstacle sensor on the diagonal left position. */
    SENSOR_JS2_DR,      /**< Obstacle sensor on the diagonal right position. */
    SENSOR_JS2_FL,      /**< Obstacle sensor on the front left position. */
    SENSOR_JS2_FR,      /**< Obstacle sensor on the front right position. */
    SENSOR_JS2_LL,      /**< Obstacle sensor on the lateral left position. */
    SENSOR_JS2_LR,      /**< Obstacle sensor on the lateral right position. */
    SENSOR_QRE_BL,      /**< Line sensor on the back left position. */
    SENSOR_QRE_BR,      /**< Line sensor on the back right position. */
    SENSOR_QRE_FL,      /**< Line sensor on the front left position. */
    SENSOR_QRE_FR,      /**< Line sensor on the front right position. */
    NUMBER_OF_SENSORS   /**< Number of sensor positions. */
} sensor_t;

/**
 * @brief Sensor operating modes.
 */
typedef enum sensor_mode {
    SENSOR_MODE_ANALOG = 0, /**< Sensor operating in analog mode. */
    SENSOR_MODE_DIGITAL,    /**< Sensor operating in digital mode. */
    NUMBER_OF_SENSOR_MODES  /**< Number of sensor modes. */
} sensor_mode_t;

/**
 * @brief Sensor configuration.
 */
typedef struct sensor_config {
    const char *name;       /**< Human-readable null-terminated Sensor name. */
    mux_channel_t channel;  /**< Sensor MUX channel. */
    sensor_mode_t mode;     /**< Sensor operating mode. */
    float threshold;        /**< Sensor @ref SENSOR_MODE_DIGITAL high threshold. */
} sensor_config_t;

/**
 * @def SENSOR_MAX_VALUE
 * @brief Sensor maximum measured value.
 *
 * **Default Value:** 1.0000f
 */
#define SENSOR_MAX_VALUE 1.0000f

/**
 * @def SENSOR_MIN_VALUE
 * @brief Sensor minimum measured value.
 *
 * **Default Value:** 0.0000f
 */
#define SENSOR_MIN_VALUE 0.0000f

/**
 * @brief Returns true if any line sensors are @ref SENSOR_MAX_VALUE .
 *
 * Line sensors are:
 * 
 * - @ref SENSOR_QRE_BL
 *
 * - @ref SENSOR_QRE_BR
 *
 * - @ref SENSOR_QRE_FL
 *
 * - @ref SENSOR_QRE_FR
 */
bool sensor_detected_line(void);

/**
 * @brief Returns true if any front obstacle sensors are @ref SENSOR_MAX_VALUE .
 *
 * Front obstacle sensors are:
 *
 * - @ref SENSOR_JS2_FL
 *
 * - @ref SENSOR_JS2_FR
 */
bool sensor_detected_obstacle_front(void);

/**
 * @brief Returns true if both front obstacle sensors are @ref SENSOR_MIN_VALUE
 * and any side obstacle sensors are @ref SENSOR_MAX_VALUE .
 *
 * Front obstacle sensors are:
 *
 * - @ref SENSOR_JS2_FL
 *
 * - @ref SENSOR_JS2_FR
 *
 * Side obstacle sensors are:
 *
 * - @ref SENSOR_JS2_DL
 *
 * - @ref SENSOR_JS2_DR
 *
 * - @ref SENSOR_JS2_LL
 *
 * - @ref SENSOR_JS2_LR
 */
bool sensor_detected_obstacle_sides(void);

/**
 * @brief Returns the sensor name.
 *
 * @param[in] sensor Sensor position.
 *
 * @return Human-readable null-terminated string representing the sensor name.
 */
const char *sensor_get_name(sensor_t sensor);

/**
 * @brief Returns the measured sensor value.
 *
 * The measured sensor value range between ``[0.0000, 1.0000]`` representing a
 * percentage of the maximum 3V3 voltage supported.
 *
 * @note When the operating mode is @ref SENSOR_MODE_DIGITAL the measured value
 * is limited to either ``0.0000`` **or** ``1.0000`` according to the defined
 * threshold.
 *
 * @param[in] sensor Sensor position.
 *
 * @return Measured sensor value
 */
float sensor_get_value(sensor_t sensor);

/**
 * @brief Initialize all the Sensors.
 */
void sensor_init(void);
