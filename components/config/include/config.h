/**
 * @file config.h
 * @brief Project configurations.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-04-28
 */

#pragma once

/**
 * @brief Defines control approaches
 *
 * @note Available options are:
 *
 * - `CONFIG_CONTROL_AUTONOMOUS`
 *
 * - `CONFIG_CONTROL_RADIO`
 */
typedef enum config_control_mode {
    CONFIG_CONTROL_AUTONOMOUS = 0,
    CONFIG_CONTROL_RADIO,
} config_control_mode_t;

#define CONFIG_CONTROL_MODE     CONFIG_CONTROL_RADIO

#define CONFIG_PROJECT_NAME     "dolphin"

#define CONFIG_WHEELBASE_M      0.1400f
#define CONFIG_WHEEL_RADIUS_M   0.0225f
