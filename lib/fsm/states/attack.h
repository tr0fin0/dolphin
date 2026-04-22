
/**
 * @brief Attack state entry handler.
 *
 * Sets visual indication upon entering `STATE_ATTACK`.
 */
void attack_entry(void);

/**
 * @brief Attack state run handler.
 *
 * Monitors radio enable signal and transitions to `STATE_SAFE` if control
 * authorization is lost.
 */
void attack_run(void);
