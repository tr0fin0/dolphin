/**
 * @file colors.h
 * @brief RGB LED colors.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-05-13
 */

#pragma once

#include <stdint.h>

/**
 * @brief LED RGB color.
 */
typedef struct color {
    uint8_t r;  /**< Red color channel ranging between 0 and 255. */
    uint8_t g;  /**< Green color channel ranging between 0 and 255. */
    uint8_t b;  /**< Blue color channel ranging between 0 and 255. */
} color_t;
