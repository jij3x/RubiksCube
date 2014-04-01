#include "cube_verifier.h"

bool valid_total_color(cube_t *cube) {
    return true;
}

bool valid_hands(cube_t *cube) {
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

bool centralpieces_solved(cube_t *cube) {
    if (cube->layers % 2 == 0)
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

bool valid_centralpieces(cube_t *cube) {
    if (!centralpieces_solved(cube))
        return false;

    lyrnum_t center = pow(cube->layers, 2) / 2;
    if (opp_color[cube->faces[YELLOW][center]] == cube->faces[WHITE][center]
            && opp_color[cube->faces[RED][center]] == cube->faces[ORANGE][center]
            && opp_color[cube->faces[BLUE][center]] == cube->faces[GREEN][center])
        return true;

    return true;
}

bool valid_edgeblocks(cube_t *cube) {
    return true;
}

bool valid_cornorblocks(cube_t *cube) {
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

bool identical_cubes(cube_t *cube1, cube_t *cube2) {
    if (cube1->layers != cube2->layers)
        return false;

    return true;
}

bool is_solved_cube(cube_t *cube) {
    if (!is_valid_cube(cube))
        return false;

    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        color_t sample = cube->faces[i][0];
        for (lyrnum_t j = 1; j < pow(cube->layers, 2); j++) {
            if (cube->hands[i][j] != sample)
                return false;
        }
    }

    return true;
}
