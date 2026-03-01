#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <debug.h>


#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Defines Control Approach
 *
 * Available options are:
 *
 * - `AUTONOMOUS`:  algorithm will decide on how to operate
 *
 * - `RADIO`:       radio receiver will decide on how to operate
 */
typedef enum {
    AUTONOMOUS,
    RADIO,
} control_mode_t;

#define PROJECT_NAME        "dolphin"

#define CONTROL_MODE        RADIO

#define COUNTDOWN_MS        5000

/**
 * @brief System-wide Debug Threshold
 * 
 * Defines which debug messages will be displayed.
 */
#define SYSTEM_DEBUG_LEVEL  DEBUG_LEVEL_WARNING




#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H__ */
