#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>


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





#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H__ */
