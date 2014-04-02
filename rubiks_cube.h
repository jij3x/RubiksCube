#ifndef RUBIKS_CUBE_H_
#define RUBIKS_CUBE_H_

#include "globe_def.h"
#include "comm_util.h"

/*
 *     +---+
 *     | Y |
 * +---+---+---+---+
 * | O | B | R | G |
 * +---+---+---+---+
 *     | W |
 *     +---+
 */

#define MAX_LAYERS (1<<12)
#define MIN_LAYERS 2

typedef uint32_t lyrnum_t;

typedef uint8_t color_t;
enum color_e {
    YELLOW = 0, WHITE, RED, ORANGE, BLUE, GREEN, TOTAL_COLORS
};

static const color_t opp_color[TOTAL_COLORS] = { WHITE, YELLOW, ORANGE, RED, GREEN, BLUE };

typedef uint8_t side_t;
enum side_e {
    TOP_SIDE = 0, BOTTOM_SIDE, RIGHT_SIDE, LEFT_SIDE, FRONT_SIDE, BACK_SIDE, TOTAL_SIDES
};

typedef uint8_t rotation_t;
enum rotation_e {
    NO_ROTATION = 0, R_90_CW, R_90_C_CW, R_180, TOTAL_ROTATIONS
};

typedef uint8_t move_t;
enum move_e {
    TOP_T90_CW = 0,
    TOP_T90_C_CW,
    TOP_T180,
    BOTTOM_T90_CW,
    BOTTOM_T90_C_CW,
    BOTTOM_T180,
    RIGHT_T90_CW,
    RIGHT_T90_C_CW,
    RIGHT_T180,
    LEFT_T90_CW,
    LEFT_T90_C_CW,
    LEFT_T180,
    FRONT_T90_CW,
    FRONT_T90_C_CW,
    FRONT_T180,
    BACK_T90_CW,
    BACK_T90_C_CW,
    BACK_T180,
    X_R90_CW,
    X_R90_C_CW,
    X_R180,
    Y_R90_CW,
    Y_R90_C_CW,
    Y_R180,
    Z_R90_CW,
    Z_R90_C_CW,
    Z_R180,
    TOTAL_MOVES
};

typedef struct cube {
    const lyrnum_t layers;
    const lyrnum_t raw_face_size;
    color_t * const * const faces;
    color_t * const * const raw_faces;
    rotation_t face_offset[TOTAL_COLORS];
    color_t *sides[TOTAL_SIDES];
    color_t side_color[TOTAL_SIDES];
} cube_t;

cube_t *create_cube(lyrnum_t layers);
void destroy_cube(cube_t *cube);
cube_t *clone_cube(cube_t *cube);
void reset_sides(cube_t *cube);

void turn_x_90_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x);
void turn_x_90_c_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x);
void turn_x_180(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x);
void turn_y_90_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y);
void turn_y_90_c_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y);
void turn_y_180(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y);
void turn_z_90_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z);
void turn_z_90_c_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z);
void turn_z_180(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z);

// relative move base on perspective, which is changeable
void move_cube(cube_t *cube, move_t move, lyrnum_t start_l, lyrnum_t end_l);

inline void rotate_coord(cube_t *cube, side_t side, lyrnum_t *restrict y, lyrnum_t *restrict x) {
    switch (cube->face_offset[cube->side_color[side]]) {
    case R_90_CW:
        SWAP(*y, *x);
        *x = cube->layers - 1 - *x;
        break;
    case R_90_C_CW:
        SWAP(*y, *x);
        *y = cube->layers - 1 - *y;
        break;
    case R_180:
        *x = cube->layers - 1 - *x;
        *y = cube->layers - 1 - *y;
    }
}

// relative cube face read
inline color_t read_face(cube_t *cube, side_t side, lyrnum_t y, lyrnum_t x) {
    assert(side < TOTAL_SIDES && y < cube->layers && x < cube->layers);

    rotate_coord(cube, side, &y, &x);
    return ((color_t (*)[cube->layers]) cube->faces[cube->side_color[side]])[y][x];
}

// relative cube face write
inline void write_face(cube_t *cube, side_t side, lyrnum_t y, lyrnum_t x, color_t color) {
    assert(side < TOTAL_SIDES && y < cube->layers && x < cube->layers);

    rotate_coord(cube, side, &y, &x);
    ((color_t (*)[cube->layers]) cube->faces[cube->side_color[side]])[y][x] = color;
}

#endif /* RUBIKS_CUBE_H_ */
