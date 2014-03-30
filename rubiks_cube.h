#ifndef RUBIKS_CUBE_H_
#define RUBIKS_CUBE_H_

#include "globe_inc.h"

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

typedef uint8_t hand_t;
enum hand_e {
    TOP_HAND = 0, BOTTOM_HAND, RIGHT_HAND, LEFT_HAND, FRONT_HAND, BACK_HAND, TOTAL_HANDS
};

typedef uint8_t rotation_t;
enum rotation_e {
    NO_ROTATION = 0, R_90_CW, R_90_C_CW, R_180, TOTAL_ROTATIONS
};

typedef uint8_t axis_t;
enum axis_e {
    AXIS_X = 0, AXIS_Y, AXIS_Z, TOTAL_AXIS
};

typedef struct cube {
    const lyrnum_t layers;
    const lyrnum_t raw_face_size;
    color_t * const * const faces;
    color_t * const * const raw_faces;
    color_t *hands[TOTAL_HANDS];
    color_t hand_idx[TOTAL_HANDS];
} cube_t;

cube_t *create_cube(lyrnum_t layers);
void destroy_cube(cube_t *cube);
cube_t *clone_cube(cube_t *cube);

void rotate_x_90_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x);
void rotate_x_90_c_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x);
void rotate_x_180(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x);
void rotate_y_90_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y);
void rotate_y_90_c_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y);
void rotate_y_180(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y);
void rotate_z_90_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z);
void rotate_z_90_c_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z);
void rotate_z_180(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z);

void flip_cube(cube_t *cube, axis_t axis, rotation_t rotation);
void move_cube(cube_t *cube, hand_t hand, rotation_t rotation, lyrnum_t start_l, lyrnum_t end_l);

void reset_coordinate(cube_t *cube);

#endif /* RUBIKS_CUBE_H_ */
