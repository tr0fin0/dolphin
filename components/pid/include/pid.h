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

#define PID_PERIOD_MIN_US 1000

/**
 * @brief Proportial Integral Derivative controller parameters.
 */
typedef struct pid {
    float kp;               /** Constaint gain for Proportional error. */
    float ki;               /** Constaint gain for Integral error. */
    float kd;               /** Constaint gain for Derivative error. */

    float current;          /** Current controlled value. */
    float setpoint;         /** Desired controlled value. */

    float error_p;          /** Current Proportional error. */
    float error_previous;   /** Previous Proportional error. */

    float error_i;          /** Current accumulative Integral error. */
    float error_i_max;      /** Maximum accumulative Integral error accepted. */

    float error_d;          /** Current Derivative error. */

    float output;
    int64_t last_time_us;
} pid_t;

/**
 * @brief Computes a Proportional Integral Derivative controller interation.
 * 
 * Calculations occur at a PID_PERIOD_MIN_MS period based on elapsed time since
 * the last update.
 */
void pid_compute(pid_t *pid);
