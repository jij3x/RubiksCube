#include "cube_verifier.h"
extern inline color_t read_face(cube_t *cube, hand_t hand, lyrnum_t y, lyrnum_t x);

static inline bool valid_hands(cube_t *cube) {
    if (opp_color[cube->hand_color[YELLOW]] != cube->hand_color[WHITE]
            || opp_color[cube->hand_color[RED]] != cube->hand_color[ORANGE]
            || opp_color[cube->hand_color[BLUE]] != cube->hand_color[GREEN])
        return false;

    for (hand_t i = 0; i < TOTAL_HANDS; i++) {
        if (cube->hands[i] != cube->faces[cube->hand_color[i]])
            return false;
    }

    return true;
}

bool valid_total_color(cube_t *cube) {
    return true;
}

static inline bool centralpieces_established(cube_t *cube) {
    if (cube->layers % 2 == 1)
        return true;

    lyrnum_t center = pow(cube->layers, 2) / 2;
    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        if (cube->faces[i][center] != cube->faces[i][center + 1]
                || cube->faces[i][center] != cube->faces[i][center - 1]
                || cube->faces[i][center] != cube->faces[i][center - 2])
            return false;
    }

    return true;
}

static inline bool valid_centralpieces(cube_t *cube) {
    if (!centralpieces_established(cube))
        return false;

    lyrnum_t center = pow(cube->layers, 2) / 2;
    if (opp_color[cube->faces[YELLOW][center]] == cube->faces[WHITE][center]
            && opp_color[cube->faces[RED][center]] == cube->faces[ORANGE][center]
            && opp_color[cube->faces[BLUE][center]] == cube->faces[GREEN][center])
        return true;

    return true;
}

static inline bool valid_edgeblocks(cube_t *cube) {
    return true;
}

static inline bool valid_cornorblocks(cube_t *cube) {
    return true;
}

bool is_valid_cube(cube_t *cube) {
    if (!valid_hands(cube))
        return false;
    if (!valid_total_color(cube))
        return false;
    if (!valid_centralpieces(cube))
        return false;
    if (!valid_edgeblocks(cube))
        return false;
    if (!valid_cornorblocks(cube))
        return false;

    return true;
}

bool is_valid_3x3x3(cube_t *cube) {
    if (cube->layers != 3 || !is_valid_cube(cube))
        return false;

    return true;
}

static inline bool identical_cubes(cube_t *cube1, cube_t *cube2) {
    assert(cube1->layers == cube2->layers);

    for (hand_t i = 0; i < TOTAL_HANDS; i++) {
        for (lyrnum_t y = 0; y < cube1->layers; y++) {
            for (lyrnum_t x = 0; x < cube1->layers; x++) {
                if (read_face(cube1, i, y, x) != read_face(cube2, i, y, x))
                    return false;
            }
        }
    }

    return true;
}

bool same_cubes(cube_t *cube1, cube_t *cube2) {
    if (cube1->layers != cube2->layers)
        return false;

    // step 1/24
    if (identical_cubes(cube1, cube2))
        return true;

    // step 2/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 3/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 4/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 5/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    move_cube(cube2, X_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 6/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 7/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 8/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 9/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    move_cube(cube2, X_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 10/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 11/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 12/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 13/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    move_cube(cube2, X_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 14/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 15/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 16/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 17/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    move_cube(cube2, X_R90_CW, 0, 0);
    move_cube(cube2, Z_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 18/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 19/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 20/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 21/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    move_cube(cube2, Z_R180, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 22/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 23/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // step 24/24
    move_cube(cube2, Y_R90_CW, 0, 0);
    if (identical_cubes(cube1, cube2))
        return true;

    // restore to original perspective
    move_cube(cube2, X_R90_C_CW, 0, 0);
    move_cube(cube2, Y_R90_C_CW, 0, 0);

    return false;
}

bool is_solved_cube(cube_t *cube) {
    if (!is_valid_cube(cube))
        return false;

    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        color_t (*face)[cube->layers] = (color_t (*)[cube->layers]) cube->faces[i];
        color_t sample = face[0][0];
        for (lyrnum_t y = 0; y < cube->layers; y++) {
            for (lyrnum_t x = 0; x < cube->layers; x++) {
                if (face[y][x] != sample)
                    return false;
            }
        }
    }

    return true;
}
