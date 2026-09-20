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
 * @def EMA_ALPHA_DEFAULT
 * @brief Exponential Moving Average with smoothing factor alpha.
 *
 * @note Smoothing factor is computed as: \f[ \alpha = \frac{2}{N + 1} \f] With
 * \f$ N \f$ being the number of periods in the average.
 *
 * **Default Value:** ``0.3333``
 */
#define EMA_ALPHA_DEFAULT 0.3333f

/**
 * @brief Computes the exponential moving average of a circular buffer.
 *
 * @param[in] buffer Pointer to a measurements buffer.
 * @param[in] capacity Capacity of the buffer.
 * @param[in] head Oldest value in the buffer.
 * @param[in] alpha Filter smoothing factor.
 *
 * @return Exponential moving average of the circular buffer as ``uint16_t``.
 */
uint16_t ema_compute_circular(
    const uint16_t *buffer, uint8_t capacity, uint8_t head, float alpha
);
