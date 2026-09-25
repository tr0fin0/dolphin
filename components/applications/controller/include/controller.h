/**
 * @file controller.h
 * @brief Motion controller functions.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-09-25
 */

#include "pwm.h"
#include <stdint.h>

/**
 * @brief Sets controller rotation.
 *
 * Uses timed intervals to control the rotation:
 *
 * - ``power < 0``: counter clockwise
 *
 * - ``power > 0``: clockwise
 *
 * @note Calls @ref esc_set_pwm_mix_neutral after the movement.
 *
 * @param[in] power Percentage of PWM power applied.
 * @param[in] duration_us Duration of movement in microseconds.
 */
void controller_set_rotation(pwm_percentage_t power, uint16_t duration_us);

/**
 * @brief Sets controller translation.
 *
 * Uses timed intervals to control the translation:
 *
 * - ``power < 0``: backwards
 *
 * - ``power > 0``: forwards
 *
 * @note Calls @ref esc_set_pwm_mix_neutral after the movement.
 *
 * @param[in] power Percentage of PWM power applied.
 * @param[in] duration_us Duration of movement in microseconds.
 */
void controller_set_translation(pwm_percentage_t power, uint16_t duration_us);
