/**
 * @file pid.h
 * @brief Proportional Integral Derivative controller abstraction.
 *
 * For more information:
 *
 * - https://en.wikipedia.org/wiki/PID_controller
 *
 * @author Guilherme Nunes Trofino
 * @date 2025-05-20
 */

#pragma once

#include <stdint.h>

/**
 * @def PID_PERIOD_MIN_US
 * @brief Minimal PID update period in microseconds.
 *
 * **Default Value:** 1000
 */
#define PID_PERIOD_MIN_US 1000

/**
 * @brief Proportial Integral Derivative controller parameters.
 */
typedef struct pid {
    float kp;               /**< Constraint gain for **Proportional** error. */
    float ki;               /**< Constraint gain for **Integral** error. */
    float kd;               /**< Constraint gain for **Derivative** error. */

    float current;          /**< Current controlled value. */
    float setpoint;         /**< Desired controlled value. */

    float error_p;          /**< Current **Proportional** error. */
    float error_previous;   /**< Previous **Proportional** error. */

    float error_i;          /**< Current accumulative **Integral** error. */
    float error_i_max;      /**< Maximum accumulative **Integral** error accepted. */

    float error_d;          /**< Current **Derivative** error. */

    float output;           /**< PID controller calculated output. */
    int64_t last_time_us;   /**< PID controller last update time in microseconds. */
} pid_t;

/**
 * @brief Computes a Proportional Integral Derivative controller iteration.
 *
 * Calculations occur at a @ref PID_PERIOD_MIN_US period based on elapsed time since
 * the last update.
 *
 * @param[in] pid PID controller parameters.
 */
void pid_compute(pid_t *pid);
