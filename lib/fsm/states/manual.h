#pragma once

/**
 * @brief Manual state entry handler.
 *
 * Sets visual indication upon entering `STATE_MANUAL`.
 */
void manual_entry(void);

/**
 * @brief Manual state run handler.
 *
 * In `RADIO_CONNECTED_ENABLE` state, reads steering and throttle radio channels
 * and forwards them mixed in a tank combination to the ESC outputs.
 *
 * If the radio becomes disabled or disconnected, transitions immediately to
 * `STATE_SAFE` for safety.
 */
void manual_run(void);

/**
 * @brief Manual state exit handler.
 *
 * Ensures both motors are driven to neutral PWM when leaving `STATE_MANUAL`,
 * preventing unintended motion during transition.
 */
void manual_exit(void);
