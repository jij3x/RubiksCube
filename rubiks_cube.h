#ifndef RUBIKS_CUBE_H_
#define RUBIKS_CUBE_H_

#include "globe_inc.h"

#define MAX_LAYERS (1<<12)
#define MIN_LAYERS 2

typedef uint32_t lcnt_t;
typedef uint8_t color_t;
enum {
    YELLOW = 0, WHITE, RED, ORANGE, BLUE, GREEN, TOTAL_COLOR, DARK = 100, BRIGHT = 101
} color_e;

typedef enum {
    RIGHT_CW90 = 0,
    RIGHT_CCW90,
    RIGHT_180,
    LEFT_CW90,
    LEFT_CCW90,
    LEFT_180,
    FRONT_CW90,
    FRONT_CCW90,
    FRONT_180,
    BACK_CW90,
    BACK_CCW90,
    BACK_180,
    TOP_CW90,
    TOP_CCW90,
    TOP_180,
    BOTTOM_CW90,
    BOTTOM_CCW90,
    BOTTOM_180,
    TOTAL_OPS
} step_t;

typedef struct {
    const lcnt_t layers;
    const lcnt_t raw_face_size;
    color_t * const top_raw, * const bottom_raw, * const right_raw, * const left_raw,
            * const front_raw, * const back_raw;
    color_t * const top, * const bottom, * const right, * const left, * const front, * const back;
} cube_t;

cube_t *create_3x3x3(void);
cube_t *create_cube(lcnt_t layers);
void destroy_cube(cube_t *cube);
cube_t *clone_cube(cube_t *cube);

void move(cube_t *cube, step_t step, lcnt_t start_l, lcnt_t end_l);
void reset_coordinate(cube_t *cube);

#endif /* RUBIKS_CUBE_H_ */
