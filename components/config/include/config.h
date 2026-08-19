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
 * Used for system identification and logging.
 *
 * **Default Value:** @ref `"dolphin"`
 */
#define CONFIG_PROJECT_NAME     "dolphin"

