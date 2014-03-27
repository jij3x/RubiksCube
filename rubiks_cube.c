#include "rubiks_cube.h"

/*
 *     +---+
 *     | Y |
 * +---+---+---+---+
 * | O | B | R | G |
 * +---+---+---+---+
 *     | W |
 *     +---+
 */
cube_t *create_cube(uint16_t layers) {
    layers = fmin(layers, MAX_LAYERS);
    layers = fmax(layers, MIN_LAYERS);

    cube_t *cube = malloc(sizeof(cube_t));

    cube->layers = layers;

    cube->top = malloc(sizeof(cube_t));
    cube->bottom = malloc(sizeof(cube_t));
    cube->front = malloc(sizeof(cube_t));
    cube->back = malloc(sizeof(cube_t));
    cube->right = malloc(sizeof(cube_t));
    cube->left = malloc(sizeof(cube_t));

    for (uint32_t i = 0; i < layers * layers; i++) {
        cube->top[i] = YELLOW;
        cube->bottom[i] = WHITE;
        cube->front[i] = BLUE;
        cube->back[i] = GREEN;
        cube->right[i] = RED;
        cube->left[i] = ORANGE;
    }

    return cube;
}

cube_t *create_3x3x3(void) {
    return create_cube(3);
}

void destroy_cube(cube_t *cube) {
    free(cube->top);
    free(cube->bottom);
    free(cube->front);
    free(cube->back);
    free(cube->right);
    free(cube->left);

    free(cube);
}

cube_t *clone_cube(cube_t *cube) {
    uint16_t layers = cube->layers;
    layers = fmin(layers, MAX_LAYERS);
    layers = fmax(layers, MIN_LAYERS);

    cube_t* clone = malloc(sizeof(cube_t));

    clone->layers = layers;

    cube->top = malloc(sizeof(cube_t));
    cube->bottom = malloc(sizeof(cube_t));
    cube->front = malloc(sizeof(cube_t));
    cube->back = malloc(sizeof(cube_t));
    cube->right = malloc(sizeof(cube_t));
    cube->left = malloc(sizeof(cube_t));

    for (uint32_t i = 0; i < layers * layers; i++) {
        clone->top[i] = cube->top[i];
        clone->bottom[i] = cube->bottom[i];
        clone->front[i] = cube->front[i];
        clone->back[i] = cube->back[i];
        clone->right[i] = cube->right[i];
        clone->left[i] = cube->left[i];
    }

    return clone;
}

void rotate_face_cw90(color_t *face_ptr, uint16_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (uint16_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (uint16_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[y_c][lvl];
            face[y_c][lvl] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y][lvl_c];
            face[y][lvl_c] = temp;
        }
    }
}

void rotate_face_ccw90(color_t *face_ptr, uint16_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (uint16_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (uint16_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[y][lvl_c];
            face[y][lvl_c] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

void rotate_face_180(color_t *face_ptr, uint16_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (uint16_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (uint16_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[lvl_c][x_c];
            face[lvl_c][x_c] = temp;

            temp = face[y][lvl_c];
            face[y][lvl_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

void x_rotate_cw90(cube_t *cube, uint16_t start_x, uint16_t end_x) {
    assert(start_x <= end_x && end_x < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;

    color_t temp;
    for (uint16_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = front[j][i];
            front[j][i] = bottom[j][i];
            bottom[j][i] = back[j_c][i_c];
            back[j_c][i_c] = top[j][i];
            top[j][i] = temp;
        }
    }

    if (start_x == 0)
        rotate_face_ccw90(cube->left, layers);
    if (end_x == layers - 1)
        rotate_face_cw90(cube->right, layers);
}

void x_rotate_ccw90(cube_t *cube, uint16_t start_x, uint16_t end_x) {
    assert(start_x <= end_x && end_x < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;

    color_t temp;
    for (uint16_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = front[j][i];
            front[j][i] = top[j][i];
            top[j][i] = back[j_c][i_c];
            back[j_c][i_c] = bottom[j][i];
            bottom[j][i] = temp;
        }
    }

    if (start_x == 0)
        rotate_face_cw90(cube->left, layers);
    if (end_x == layers - 1)
        rotate_face_ccw90(cube->right, layers);
}

void x_rotate_180(cube_t *cube, uint16_t start_x, uint16_t end_x) {
    assert(start_x <= end_x && end_x < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;

    color_t temp;
    for (uint16_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = top[j][i];
            top[j][i] = bottom[j][i];
            bottom[j][i] = temp;

            temp = front[j][i];
            front[j][i] = back[j_c][i_c];
            back[j_c][i_c] = temp;
        }
    }

    if (start_x == 0)
        rotate_face_180(cube->left, layers);
    if (end_x == layers - 1)
        rotate_face_180(cube->right, layers);
}

void y_rotate_cw90(cube_t *cube, uint16_t start_y, uint16_t end_y) {
    assert(start_y < end_y && end_y < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;

    color_t temp;
    for (uint16_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = front[i][j];
            front[i][j] = right[i][j];
            right[i][j] = back[i][j];
            back[i][j] = left[i][j];
            left[i][j] = temp;
        }
    }

    if (start_y == 0)
        rotate_face_cw90(cube->top, layers);
    if (end_y == layers - 1)
        rotate_face_ccw90(cube->bottom, layers);
}

void y_rotate_ccw90(cube_t *cube, uint16_t start_y, uint16_t end_y) {
    assert(start_y < end_y && end_y < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;

    color_t temp;
    for (uint16_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = front[i][j];
            front[i][j] = left[i][j];
            left[i][j] = back[i][j];
            back[i][j] = right[i][j];
            right[i][j] = temp;
        }
    }

    if (start_y == 0)
        rotate_face_ccw90(cube->top, layers);
    if (end_y == layers - 1)
        rotate_face_cw90(cube->bottom, layers);
}

void y_rotate_180(cube_t *cube, uint16_t start_y, uint16_t end_y) {
    assert(start_y < end_y && end_y < cube->layers);
    assert(start_y < end_y && end_y < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;

    color_t temp;
    for (uint16_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = front[i][j];
            front[i][j] = back[i][j];
            back[i][j] = temp;

            temp = left[i][j];
            left[i][j] = right[i][j];
            right[i][j] = temp;
        }
    }

    if (start_y == 0)
        rotate_face_180(cube->top, layers);
    if (end_y == layers - 1)
        rotate_face_180(cube->bottom, layers);
}

void z_rotate_cw90(cube_t *cube, uint16_t start_z, uint16_t end_z) {
    assert(start_z < end_z && end_z < cube->layers);
    assert(start_z < end_z && end_z < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;

    color_t temp;
    for (uint16_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = right[j][i];
            right[j][i] = top[i_c][j];
            top[i_c][j] = left[j_c][i_c];
            left[j_c][i_c] = bottom[i][j_c];
            bottom[i][j_c] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_cw90(cube->front, layers);
    if (end_z == layers - 1)
        rotate_face_ccw90(cube->back, layers);
}

void z_rotate_ccw90(cube_t *cube, uint16_t start_z, uint16_t end_z) {
    assert(start_z < end_z && end_z < cube->layers);
    assert(start_z < end_z && end_z < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;

    color_t temp;
    for (uint16_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = right[j][i];
            right[j][i] = bottom[i][j_c];
            bottom[i][j_c] = left[j_c][i_c];
            left[j_c][i_c] = top[i_c][j];
            top[i_c][j] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_ccw90(cube->front, layers);
    if (end_z == layers - 1)
        rotate_face_cw90(cube->back, layers);
}

void z_rotate_180(cube_t *cube, uint16_t start_z, uint16_t end_z) {
    assert(start_z < end_z && end_z < cube->layers);
    assert(start_z < end_z && end_z < cube->layers);

    uint16_t layers = cube->layers;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;

    color_t temp;
    for (uint16_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (uint16_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = right[j][i];
            right[j][i] = left[j_c][i_c];
            left[j_c][i_c] = temp;

            temp = top[i_c][j];
            top[i_c][j] = bottom[i][j_c];
            bottom[i][j_c] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_180(cube->front, layers);
    if (end_z == layers - 1)
        rotate_face_180(cube->back, layers);
}

void rotate_right_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    uint16_t start_x = cube->layers - 1 - end_l, end_x = cube->layers - 1 - start_l;
    x_rotate_cw90(cube, start_x, end_x);
}

void rotate_right_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    uint16_t start_x = cube->layers - 1 - end_l, end_x = cube->layers - 1 - start_l;
    x_rotate_ccw90(cube, start_x, end_x);
}

void rotate_right_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    uint16_t start_x = cube->layers - 1 - end_l, end_x = cube->layers - 1 - start_l;
    x_rotate_180(cube, start_x, end_x);
}

void rotate_left_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    x_rotate_cw90(cube, start_l, end_l);
}

void rotate_left_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    x_rotate_ccw90(cube, start_l, end_l);
}

void rotate_left_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    x_rotate_180(cube, start_l, end_l);
}

void rotate_front_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
}

void rotate_front_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
}

void rotate_front_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
}

void rotate_back_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
}

void rotate_back_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
}

void rotate_back_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
}

void rotate_top_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    y_rotate_ccw90(cube, start_l, end_l);
}

void rotate_top_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    y_rotate_ccw90(cube, start_l, end_l);
}

void rotate_top_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    y_rotate_ccw90(cube, start_l, end_l);
}

void rotate_bottom_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    uint16_t start_y = cube->layers - 1 - end_l, end_y = cube->layers - 1 - start_l;
    y_rotate_cw90(cube, start_y, end_y);
}

void rotate_bottom_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    uint16_t start_y = cube->layers - 1 - end_l, end_y = cube->layers - 1 - start_l;
    y_rotate_ccw90(cube, start_y, end_y);
}

void rotate_bottom_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    uint16_t start_y = cube->layers - 1 - end_l, end_y = cube->layers - 1 - start_l;
    y_rotate_180(cube, start_y, end_y);
}

typedef void (*cube_move)(cube_t *, uint16_t, uint16_t);

static cube_move op_list[TOTAL_OPS] = { &rotate_right_cw90, &rotate_right_ccw90, &rotate_right_180,
        &rotate_left_cw90, &rotate_left_ccw90, &rotate_left_180, &rotate_top_cw90,
        &rotate_top_ccw90, &rotate_top_180, &rotate_bottom_cw90, &rotate_bottom_ccw90,
        &rotate_bottom_180, &rotate_front_cw90, &rotate_front_ccw90, &rotate_front_180,
        &rotate_back_cw90, &rotate_back_ccw90, &rotate_back_180 };

void move(cube_t *cube, step_t step, uint16_t start_l, uint16_t end_l) {
    if (start_l > end_l)
        SWAP(start_l, end_l);
    end_l = fmin(end_l, cube->layers - 1);

    op_list[step](cube, start_l, end_l);
}
