/**
 * @file config.h
 * @brief Project configurations.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-04-28
 */

#pragma once

/**
 * @brief Available control approaches.
 *
 * @note Control approaches are mutual exclusive, meaning only one can be chosen
 * at a time.
 */
typedef enum config_control_mode {
    CONFIG_CONTROL_AUTONOMOUS = 0, /**< Robot controlled via algorithms. */
    CONFIG_CONTROL_RADIO,          /**< Robot controlled via radio receiver. */
} config_control_mode_t;

/**
 * @def CONFIG_CONTROL_MODE
 * @brief Current control approach.
 *
 * Determines how the robot is controlled at runtime.
 *
 * **Default Value:** @ref CONFIG_CONTROL_RADIO
 */
#define CONFIG_CONTROL_MODE     CONFIG_CONTROL_RADIO

/**
 * @def CONFIG_PROJECT_NAME
 * @brief Project name identifier.
 *
 * Human-readable null-terminated system identification.
 *
 * **Default Value:** "dolphin"
 */
#define CONFIG_PROJECT_NAME     "dolphin"

/**
 * @def CONFIG_WHEEL_RADIUS_M
 * @brief Project wheel radius in meters
 *
 * **Default Value:** 0.0225
 */
#define CONFIG_WHEEL_RADIUS_M   0.0225f

/**
 * @def CONFIG_WHEELBASE_M
 * @brief Project distance between wheels center in meters.
 *
 * **Default Value:** 0.1400
 */
#define CONFIG_WHEELBASE_M      0.1400f
