#ifndef __OPENING_H__
#define __OPENING_H__



/**
 * @brief Opening state entry handler.
 *
 * Sets visual indication upon entering `STATE_OPENING`.
 */
void opening_entry(void);

/**
 * @brief Opening state run handler.
 *
 * Reads throttle value in 3 sequencial steps to determine the opening move
 * requested among the defined below:
 */
void opening_run(void);




#endif  /* __OPENING_H__ */
