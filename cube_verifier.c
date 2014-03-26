#include "cube_verifier.h"

bool_t valid_total_color(cube_t *cube) {
    int count_tbl[TOTAL_COLOR], layers = cube->layers;
    color_t i;
    for (i = 0; i < TOTAL_COLOR; i++) {
        count_tbl[i] = layers * layers;
    }

    for (i = 0; i < TOTAL_COLOR; i++) {
        color_t *face_ptr = (color_t *) cube->faces[i];
        uint32_t j;
        for (j = 0; j < layers * layers; j++) {
            count_tbl[face_ptr[j]]--;
        }
    }

    for (i = 0; i < TOTAL_COLOR; i++) {
        if (count_tbl[i] != 0)
            return false;
    }
    return true;
}

bool_t opposite_color(color_t c1, color_t c2) {
    if ((c1 == WHITE && c2 == YELLOW) || (c1 == YELLOW && c2 == WHITE))
        return true;
    if ((c1 == RED && c2 == ORANGE) || (c1 == ORANGE && c2 == RED))
        return true;
    if ((c1 == BLUE && c2 == GREEN) || (c1 == GREEN && c2 == BLUE))
        return true;

    return false;
}

bool_t valid_centralpieces(cube_t *cube) {
    uint16_t layers = cube->layers;
    uint32_t center = layers * layers / 2;
    if (layers % 2 == 0)
        return false;

    if (opposite_color(cube->left[center], cube->right[center])
            && opposite_color(cube->front[center], cube->back[center])
            && opposite_color(cube->top[center], cube->bottom[center]))
        return true;

    return true;
}

bool_t is_valid_3x3x3(cube_t *cube) {
    if (!valid_total_color(cube) || !valid_centralpieces(cube) || cube->layers != 3)
        return false;

    return true;
}

bool_t centralpieces_solved(cube_t *cube) {
    uint16_t layers = cube->layers;
    uint32_t center = layers * layers / 2;
    if (layers % 2 == 0)
        return true;

    color_t it;
    for (it = 0; it < TOTAL_COLOR; it++) {
        if (!(cube->faces[it][center] == cube->faces[it][center + 1]
                && cube->faces[it][center] == cube->faces[it][center - 1]
                && cube->faces[it][center] == cube->faces[it][center - 2]))
            return false;
    }
    return true;
}

void reset_coordinate(cube_t *cube) {
    uint16_t layers = cube->layers;
    uint32_t center = layers * layers / 2;
    if (layers % 2 == 0 && !centralpieces_solved(cube))
        return;

    switch (cube->front[center]) {
    case RED:
        move(cube, BOTTOM_CW90, 0, cube->layers - 1);
        break;
    case ORANGE:
        move(cube, BOTTOM_CCW90, 0, cube->layers - 1);
        break;
    case WHITE:
        move(cube, RIGHT_CCW90, 0, cube->layers - 1);
        break;
    case YELLOW:
        move(cube, RIGHT_CW90, 0, cube->layers - 1);
        break;
    case GREEN:
        move(cube, BOTTOM_180, 0, cube->layers - 1);
        break;
    default:
        break;
    }

    switch (cube->top[center]) {
    case RED:
        move(cube, FRONT_CW90, 0, cube->layers - 1);
        break;
    case ORANGE:
        move(cube, FRONT_CCW90, 0, cube->layers - 1);
        break;
    case BLUE:
        move(cube, RIGHT_CCW90, 0, cube->layers - 1);
        break;
    case GREEN:
        move(cube, RIGHT_CW90, 0, cube->layers - 1);
        break;
    case YELLOW:
        move(cube, RIGHT_180, 0, cube->layers - 1);
        break;
    default:
        break;
    }

    switch (cube->right[center]) {
    case BLUE:
        move(cube, BOTTOM_CCW90, 0, cube->layers - 1);
        break;
    case GREEN:
        move(cube, BOTTOM_CW90, 0, cube->layers - 1);
        break;
    case WHITE:
        move(cube, RIGHT_CCW90, 0, cube->layers - 1);
        break;
    case YELLOW:
        move(cube, RIGHT_CW90, 0, cube->layers - 1);
        break;
    case ORANGE:
        move(cube, BOTTOM_180, 0, cube->layers - 1);
        break;
    default:
        break;
    }
}
