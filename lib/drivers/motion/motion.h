#ifndef __MOTION_H__
#define __MOTION_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Move rotation angle in degrees.
 *
 * With:
 *
 * - positive: clockwise sense
 *
 * - negative: counterclockwise sense
 */
typedef int16_t angle_t;

/**
 * @brief Move distance angle in degrees.
 *
 * With:
 *
 * - positive: forwards direction
 *
 * - negative: backwards direction
 */
typedef int16_t distance_t;

/**
 * @brief Move motors so robot performs a rotation.
 *
 * @param angle rotation angle in degree.
 */
void motion_rotate(angle_t angle);

/**
 * @brief Move motors so robot performs a translation.
 *
 * @param distance translation distance in centimeters.
 */
void motion_translation(distance_t distance);

#ifdef __cplusplus
}
#endif

#endif /* __MOTION_H__ */
