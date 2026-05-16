/**
 * @file config.h
 * @brief Project configurations.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-04-28
 */

#pragma once

/**
 * @brief Defines Control Approach
 *
 * @note Available options are:
 *
 * - `AUTONOMOUS`
 *
 * - `RADIO`
 */
typedef enum {
    AUTONOMOUS = 0,
    RADIO,
} control_mode_t;

#define PROJECT_NAME    "dolphin"

#define CONTROL_MODE    RADIO
