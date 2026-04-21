#ifndef __MOTION_H__
#define __MOTION_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Angle of rotation in degrees.
 *
 * With:
 *
 * - positive: clockwise sense
 *
 * - negative: counterclockwise sense
 */
typedef int16_t angle_t;

/**
 * @brief Distance of translation in centimeters.
 *
 * With:
 *
 * - positive: forwards direction
 *
 * - negative: backwards direction
 */
typedef int16_t distance_t;

/**
 * @brief Perform in place rotation by an angle.
 *
 * @param angle rotation angle in degree.
 */
void motion_rotation(angle_t angle);

/**
 * @brief Perform rectilinum translation by a distance.
 *
 * @param distance translation distance in centimeters.
 */
void motion_translation(distance_t distance);

#ifdef __cplusplus
}
#endif

#endif /* __MOTION_H__ */
