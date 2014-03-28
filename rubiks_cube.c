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

color_t *get_algn_addr(color_t *raw_addr) {
    uintptr_t aligned_addr = ~((uintptr_t) ALIGNMENT - 1);
    aligned_addr &= ((uintptr_t) raw_addr + ALIGNMENT - 1);

    return (color_t *) raw_addr;
}

void fill_face(color_t *face_ptr, color_t color, lcnt_t layers) {
    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

    for (lcnt_t i = 0; i < layers; i++) {
        for (lcnt_t j = 0; j < layers; j++) {
            face[i][j] = color;
        }
    }
}

void copy_face(color_t *src_ptr, color_t *tgt_ptr, lcnt_t layers) {
    color_t (*source)[layers] = (color_t (*)[layers]) src_ptr;
    color_t (*target)[layers] = (color_t (*)[layers]) tgt_ptr;

    for (lcnt_t i = 0; i < layers; i++) {
        for (lcnt_t j = 0; j < layers; j++) {
            target[i][j] = source[i][j];
        }
    }
}

void rotate_face_cw90(color_t *face_ptr, lcnt_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (lcnt_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (lcnt_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[y_c][lvl];
            face[y_c][lvl] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y][lvl_c];
            face[y][lvl_c] = temp;
        }
    }
}

void rotate_face_ccw90(color_t *face_ptr, lcnt_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (lcnt_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (lcnt_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[y][lvl_c];
            face[y][lvl_c] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

void rotate_face_180(color_t *face_ptr, lcnt_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (lcnt_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (lcnt_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[lvl_c][x_c];
            face[lvl_c][x_c] = temp;

            temp = face[y][lvl_c];
            face[y][lvl_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

cube_t *create_cube(lcnt_t layers) {
    layers = fmin(layers, MAX_LAYERS);
    layers = fmax(layers, MIN_LAYERS);

    lcnt_t raw_face_size = pow(layers, 2) * sizeof(color_t) + ALIGNMENT - 1;

    color_t *top_raw = malloc(raw_face_size);
    color_t *bottom_raw = malloc(raw_face_size);
    color_t *right_raw = malloc(raw_face_size);
    color_t *left_raw = malloc(raw_face_size);
    color_t *front_raw = malloc(raw_face_size);
    color_t *back_raw = malloc(raw_face_size);
    if (top_raw == NULL || bottom_raw == NULL || left_raw == NULL || right_raw == NULL
            || front_raw == NULL || back_raw == NULL)
        abort();

    color_t *top = get_algn_addr(top_raw);
    color_t *bottom = get_algn_addr(bottom_raw);
    color_t *right = get_algn_addr(right_raw);
    color_t *left = get_algn_addr(left_raw);
    color_t *front = get_algn_addr(front_raw);
    color_t *back = get_algn_addr(back_raw);

    cube_t init_cube = { .layers = layers, .raw_face_size = raw_face_size, .top_raw = top_raw,
            .bottom_raw = bottom_raw, .right_raw = right_raw, .left_raw = left_raw, .front_raw =
                    front, .back_raw = back, .top = top, .bottom = bottom, .right = right, .left =
                    left, .front = front, .back = back };

    cube_t *cube = malloc(sizeof(cube_t));
    if (cube == NULL)
        abort();
    memcpy(cube, &init_cube, sizeof(cube_t));

    fill_face(cube->top, YELLOW, cube->layers);
    fill_face(cube->bottom, WHITE, cube->layers);
    fill_face(cube->right, RED, cube->layers);
    fill_face(cube->left, ORANGE, cube->layers);
    fill_face(cube->front, BLUE, cube->layers);
    fill_face(cube->back, GREEN, cube->layers);

    return cube;
}

cube_t *create_3x3x3(void) {
    return create_cube(3);
}

void destroy_cube(cube_t *cube) {
    free(cube->top);
    free(cube->bottom);
    free(cube->right);
    free(cube->left);
    free(cube->front);
    free(cube->back);

    free(cube);
}

cube_t *clone_cube(cube_t *cube) {
    cube_t *clone = create_cube(cube->layers);

    copy_face(cube->top, clone->top, clone->layers);
    copy_face(cube->bottom, clone->bottom, clone->layers);
    copy_face(cube->right, clone->right, clone->layers);
    copy_face(cube->left, clone->left, clone->layers);
    copy_face(cube->front, clone->front, clone->layers);
    copy_face(cube->back, clone->back, clone->layers);

    return clone;
}

void x_rotate_cw90(cube_t *cube, lcnt_t start_x, lcnt_t end_x) {
    assert(start_x <= end_x && end_x < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;

    color_t temp;
    for (lcnt_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
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

void x_rotate_ccw90(cube_t *cube, lcnt_t start_x, lcnt_t end_x) {
    assert(start_x <= end_x && end_x < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;

    color_t temp;
    for (lcnt_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
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

void x_rotate_180(cube_t *cube, lcnt_t start_x, lcnt_t end_x) {
    assert(start_x <= end_x && end_x < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;

    color_t temp;
    for (lcnt_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
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

void y_rotate_cw90(cube_t *cube, lcnt_t start_y, lcnt_t end_y) {
    assert(start_y < end_y && end_y < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;

    color_t temp;
    for (lcnt_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
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

void y_rotate_ccw90(cube_t *cube, lcnt_t start_y, lcnt_t end_y) {
    assert(start_y < end_y && end_y < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;

    color_t temp;
    for (lcnt_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
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

void y_rotate_180(cube_t *cube, lcnt_t start_y, lcnt_t end_y) {
    assert(start_y < end_y && end_y < cube->layers);
    assert(start_y < end_y && end_y < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*front)[layers] = (color_t (*)[layers]) cube->front;
    color_t (*back)[layers] = (color_t (*)[layers]) cube->back;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;

    color_t temp;
    for (lcnt_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
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

void z_rotate_cw90(cube_t *cube, lcnt_t start_z, lcnt_t end_z) {
    assert(start_z < end_z && end_z < cube->layers);
    assert(start_z < end_z && end_z < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;

    color_t temp;
    for (lcnt_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = left[j][i];
            left[j][i] = bottom[i_c][j];
            bottom[i_c][j] = right[j_c][i_c];
            right[j_c][i_c] = top[i][j_c];
            top[i][j_c] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_ccw90(cube->back, layers);
    if (end_z == layers - 1)
        rotate_face_cw90(cube->front, layers);
}

void z_rotate_ccw90(cube_t *cube, lcnt_t start_z, lcnt_t end_z) {
    assert(start_z < end_z && end_z < cube->layers);
    assert(start_z < end_z && end_z < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;

    color_t temp;
    for (lcnt_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = left[j][i];
            left[j][i] = top[i][j_c];
            top[i][j_c] = right[j_c][i_c];
            right[j_c][i_c] = bottom[i_c][j];
            bottom[i_c][j] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_cw90(cube->front, layers);
    if (end_z == layers - 1)
        rotate_face_ccw90(cube->back, layers);
}

void z_rotate_180(cube_t *cube, lcnt_t start_z, lcnt_t end_z) {
    assert(start_z < end_z && end_z < cube->layers);
    assert(start_z < end_z && end_z < cube->layers);

    lcnt_t layers = cube->layers;
    color_t (*left)[layers] = (color_t (*)[layers]) cube->left;
    color_t (*right)[layers] = (color_t (*)[layers]) cube->right;
    color_t (*top)[layers] = (color_t (*)[layers]) cube->top;
    color_t (*bottom)[layers] = (color_t (*)[layers]) cube->bottom;

    color_t temp;
    for (lcnt_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (lcnt_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = left[j][i];
            left[j][i] = right[j_c][i_c];
            right[j_c][i_c] = temp;

            temp = top[i][j_c];
            top[i][j_c] = bottom[i_c][j];
            bottom[i_c][j] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_180(cube->back, layers);
    if (end_z == layers - 1)
        rotate_face_180(cube->front, layers);
}

void rotate_top_cw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    y_rotate_ccw90(cube, start_l, end_l);
}

void rotate_top_ccw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    y_rotate_ccw90(cube, start_l, end_l);
}

void rotate_top_180(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    y_rotate_ccw90(cube, start_l, end_l);
}

void rotate_bottom_cw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_y = cube->layers - 1 - end_l, end_y = cube->layers - 1 - start_l;
    y_rotate_cw90(cube, start_y, end_y);
}

void rotate_bottom_ccw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_y = cube->layers - 1 - end_l, end_y = cube->layers - 1 - start_l;
    y_rotate_ccw90(cube, start_y, end_y);
}

void rotate_bottom_180(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_y = cube->layers - 1 - end_l, end_y = cube->layers - 1 - start_l;
    y_rotate_180(cube, start_y, end_y);
}

void rotate_right_cw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_x = cube->layers - 1 - end_l, end_x = cube->layers - 1 - start_l;
    x_rotate_cw90(cube, start_x, end_x);
}

void rotate_right_ccw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_x = cube->layers - 1 - end_l, end_x = cube->layers - 1 - start_l;
    x_rotate_ccw90(cube, start_x, end_x);
}

void rotate_right_180(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_x = cube->layers - 1 - end_l, end_x = cube->layers - 1 - start_l;
    x_rotate_180(cube, start_x, end_x);
}

void rotate_left_cw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    x_rotate_cw90(cube, start_l, end_l);
}

void rotate_left_ccw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    x_rotate_ccw90(cube, start_l, end_l);
}

void rotate_left_180(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    x_rotate_180(cube, start_l, end_l);
}

void rotate_front_cw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_z = cube->layers - 1 - end_l, end_z = cube->layers - 1 - start_l;
    z_rotate_cw90(cube, start_z, end_z);
}

void rotate_front_ccw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_z = cube->layers - 1 - end_l, end_z = cube->layers - 1 - start_l;
    z_rotate_ccw90(cube, start_z, end_z);
}

void rotate_front_180(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    lcnt_t start_z = cube->layers - 1 - end_l, end_z = cube->layers - 1 - start_l;
    z_rotate_180(cube, start_z, end_z);
}

void rotate_back_cw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    z_rotate_cw90(cube, start_l, end_l);
}

void rotate_back_ccw90(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    z_rotate_ccw90(cube, start_l, end_l);
}

void rotate_back_180(cube_t *cube, lcnt_t start_l, lcnt_t end_l) {
    assert(start_l <= end_l && end_l < cube->layers);

    z_rotate_180(cube, start_l, end_l);
}

typedef void (*cube_move)(cube_t *, lcnt_t, lcnt_t);

static cube_move op_list[TOTAL_OPS] = { &rotate_right_cw90, &rotate_right_ccw90, &rotate_right_180,
        &rotate_left_cw90, &rotate_left_ccw90, &rotate_left_180, &rotate_top_cw90,
        &rotate_top_ccw90, &rotate_top_180, &rotate_bottom_cw90, &rotate_bottom_ccw90,
        &rotate_bottom_180, &rotate_front_cw90, &rotate_front_ccw90, &rotate_front_180,
        &rotate_back_cw90, &rotate_back_ccw90, &rotate_back_180 };

void move(cube_t *cube, step_t step, lcnt_t start_l, lcnt_t end_l) {
    if (start_l > end_l)
        SWAP(start_l, end_l);
    end_l = fmin(end_l, cube->layers - 1);

    op_list[step](cube, start_l, end_l);
}
