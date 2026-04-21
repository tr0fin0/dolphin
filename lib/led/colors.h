/**
 * @brief Predefined RGB color macros for convenience.
 *
 * Use `COLORS[][]` and `COLORS_COUNT` for iteration in tests.
 */

#ifndef __COLORS_H__
#define __COLORS_H__

#ifdef __cplusplus
extern "C" {
#endif



#define COLOR_RED           255,   0,   0
#define COLOR_ORANGE_DARK   254,  23,   0
#define COLOR_ORANGE_LIGHT  255,  48,   0
#define COLOR_YELLOW        255, 115,   0
#define COLOR_GREEN_LIME    163, 251,   0
#define COLOR_GREEN_LIGHT     0, 220,  20
#define COLOR_GREEN           0, 230,   0
#define COLOR_EMERALD         0, 250,  40
#define COLOR_CIAN            0, 255, 255
#define COLOR_BLUE_LIGHT      0,  90, 255
#define COLOR_BLUE            0,   0, 255
#define COLOR_PURPLE        252,   3, 232
#define COLOR_PINK          240,   0,  80
#define COLOR_SCARLET       255,   0,   6
#define COLOR_WHITE         200, 200, 200
#define COLOR_OFF             0,   0,   0

extern const uint8_t COLORS[][3];
extern const size_t COLORS_COUNT;



#ifdef __cplusplus
}
#endif

#endif /* __COLORS_H__ */
