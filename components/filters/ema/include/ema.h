/**
 * @file ema.h
 * @brief Exponential Moving Average filter.
 *
 * @note https://en.wikipedia.org/wiki/Exponential_smoothing .
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-09
 */

#pragma once

#include <stdint.h>

/**
 * @def EMA_ALPHA_CALCULATED
 * @brief EMA smoothing factor alpha calculated with buffer capacity.
 *
 * @note Smoothing factor is calculated as: \f[ \alpha = \frac{2}{N + 1} \f]
 * With \f$ N \f$ being the buffer capacity.
 */
#define EMA_ALPHA_CALCULATED -1.0f

/**
 * @brief Computes the exponential moving average of a circular buffer.
 *
 * @param[in] buffer Pointer to a measurements buffer.
 * @param[in] capacity Number of measurements in the buffer.
 * @param[in] head Position of the oldest value in the buffer.
 * @param[in] alpha Filter smoothing factor between 0 and 1.
 *
 * @note If alpha value is not in ``[0, 1]``, @ref EMA_ALPHA_CALCULATED applies.
 *
 * @return Exponential moving average of the circular buffer as ``uint16_t``.
 */
uint16_t ema_compute_circular(
    const uint16_t *buffer, uint8_t capacity, uint8_t head, float alpha
);
