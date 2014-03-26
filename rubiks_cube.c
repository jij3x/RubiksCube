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
    cube_t *cube = malloc(sizeof(cube_t));

    layers = MIN(layers, MAX_LAYERS);
    cube->layers = layers;
    color_t i;
    for (i = 0; i < TOTAL_COLOR; i++) {
        cube->faces[i] = malloc(layers * layers * sizeof(color_t));
        color_t *face_ptr = (color_t *) cube->faces[i];
        uint16_t j;
        for (j = 0; j < layers * layers; j++) {
            face_ptr[j] = i;
        }
    }
    cube->top = cube->faces[YELLOW];
    cube->bottom = cube->faces[WHITE];
    cube->front = cube->faces[BLUE];
    cube->back = cube->faces[GREEN];
    cube->right = cube->faces[RED];
    cube->left = cube->faces[ORANGE];

    return cube;
}

cube_t *create_3x3x3(void) {
    return create_cube(3);
}

void destroy_cube(cube_t *cube) {
    color_t i;
    for (i = 0; i < TOTAL_COLOR; i++) {
        free(cube->faces[i]);
    }

    free(cube);
}

cube_t *clone_cube(cube_t *cube) {
    cube_t* clone = malloc(sizeof(cube_t));

    uint16_t layers = MIN(cube->layers, MAX_LAYERS);
    clone->layers = layers;
    color_t i;
    for (i = 0; i < TOTAL_COLOR; i++) {
        clone->faces[i] = malloc(layers * layers * sizeof(color_t));
        color_t *face_ptr = (color_t *) clone->faces[i];
        uint32_t j;
        for (j = 0; j < layers * layers; j++) {
            face_ptr[j] = i;
        }

        if (cube->top == cube->faces[i])
            clone->back = clone->faces[i];
        else if (cube->bottom == cube->faces[i])
            clone->back = clone->faces[i];
        else if (cube->front == cube->faces[i])
            clone->front = clone->faces[i];
        else if (cube->back == cube->faces[i])
            clone->back = clone->faces[i];
        else if (cube->right == cube->faces[i])
            clone->right = clone->faces[i];
        else if (cube->left == cube->faces[i])
            clone->left = clone->faces[i];
    }

    return clone;
}

typedef enum {
    EAST, SOUTH, WEST, NORTH
} direction_t;

void row2row(color_t *src_ptr, uint16_t src_y, uint16_t src_x, direction_t src_dir,
        color_t *tgt_ptr, uint16_t tgt_y, uint16_t tgt_x, direction_t tgt_dir, uint16_t layers) {
    assert(layers > 0 && layers <= MAX_LAYERS);
    assert((src_dir == EAST && src_x == 0) || (src_dir == WEST && src_x == layers - 1));
    assert((tgt_dir == EAST && tgt_x == 0) || (tgt_dir == WEST && tgt_x == layers - 1));

    int16_t src_step = (src_dir == EAST ? 1 : -1);
    int16_t tgt_step = (tgt_dir == EAST ? 1 : -1);
    uint16_t src_end = (src_dir == EAST ? layers - 1 : 0);
    color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
    color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
    uint16_t i, j;
    bool_t reached = false;
    for (i = src_x, j = tgt_x; !reached; i += src_step, j += tgt_step) {
        tgt_face[tgt_y][j] = src_face[src_y][i];
        reached = (i == src_end ? true : false);
    }
}

void row2col(color_t *src_ptr, uint16_t src_y, uint16_t src_x, direction_t src_dir,
        color_t *tgt_ptr, uint16_t tgt_y, uint16_t tgt_x, direction_t tgt_dir, uint16_t layers) {
    assert(layers > 0 && layers <= MAX_LAYERS);
    assert((src_dir == EAST && src_x == 0) || (src_dir == WEST && src_x == layers - 1));
    assert((tgt_dir == SOUTH && tgt_y == 0) || (tgt_dir == NORTH && tgt_y == layers - 1));

    int16_t src_step = (src_dir == EAST ? 1 : -1);
    int16_t tgt_step = (tgt_dir == SOUTH ? 1 : -1);
    uint16_t src_end = (src_dir == EAST ? layers - 1 : 0);
    color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
    color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
    uint16_t i, j;
    bool_t reached = false;
    for (i = src_x, j = tgt_y; !reached; i += src_step, j += tgt_step) {
        tgt_face[j][tgt_x] = src_face[src_y][i];
        reached = (i == src_end ? true : false);
    }
}

void col2row(color_t *src_ptr, uint16_t src_y, uint16_t src_x, direction_t src_dir,
        color_t *tgt_ptr, uint16_t tgt_y, uint16_t tgt_x, direction_t tgt_dir, uint16_t layers) {
    assert(layers > 0 && layers <= MAX_LAYERS);
    assert((src_dir == SOUTH && src_y == 0) || (src_dir == NORTH && src_y == layers - 1));
    assert((tgt_dir == EAST && tgt_x == 0) || (tgt_dir == WEST && tgt_x == layers - 1));

    int16_t src_step = (src_dir == SOUTH ? 1 : -1);
    int16_t tgt_step = (tgt_dir == EAST ? 1 : -1);
    uint16_t src_end = (src_dir == SOUTH ? layers - 1 : 0);
    color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
    color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
    uint16_t i, j;
    bool_t reached = false;
    for (i = src_y, j = tgt_x; !reached; i += src_step, j += tgt_step) {
        tgt_face[tgt_y][j] = src_face[i][src_x];
        reached = (i == src_end ? true : false);
    }
}

void col2col(color_t *src_ptr, uint16_t src_y, uint16_t src_x, direction_t src_dir,
        color_t *tgt_ptr, uint16_t tgt_y, uint16_t tgt_x, direction_t tgt_dir, uint16_t layers) {
    assert(layers > 0 && layers <= MAX_LAYERS);
    assert((src_dir == SOUTH && src_y == 0) || (src_dir == NORTH && src_y == layers - 1));
    assert((tgt_dir == SOUTH && tgt_y == 0) || (tgt_dir == NORTH && tgt_y == layers - 1));

    int16_t src_step = (src_dir == SOUTH ? 1 : -1);
    int16_t tgt_step = (tgt_dir == SOUTH ? 1 : -1);
    uint16_t src_end = (src_dir == SOUTH ? layers - 1 : 0);
    color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
    color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
    uint16_t i, j;
    bool_t reached = false;
    for (i = src_y, j = tgt_y; !reached; i += src_step, j += tgt_step) {
        tgt_face[j][tgt_x] = src_face[i][src_x];
        reached = (i == src_end ? true : false);
    }
}

void rotate_face_cw90(color_t *face_ptr, uint16_t layers) {
    assert(layers > 0 && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

    uint16_t lvl, lvl_c, x, x_c, y, y_c;
    for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (x = y = lvl, x_c = y_c = lvl_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            color_t temp = face[lvl][x];
            face[lvl][x] = face[y_c][lvl];
            face[y_c][lvl] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y][lvl_c];
            face[y][lvl_c] = temp;
        }
    }
}

void rotate_face_ccw90(color_t *face_ptr, uint16_t layers) {
    assert(layers > 0 && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

    uint16_t lvl, lvl_c, x, x_c, y, y_c;
    for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (x = y = lvl, x_c = y_c = lvl_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            color_t temp = face[lvl][x];
            face[lvl][x] = face[y][lvl_c];
            face[y][lvl_c] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

void rotate_face_180(color_t *face_ptr, uint16_t layers) {
    assert(layers > 0 && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

    uint16_t lvl, lvl_c, x, x_c, y, y_c;
    for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (x = y = lvl, x_c = y_c = lvl_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            color_t temp = face[lvl][x];
            face[lvl][x] = face[lvl_c][x_c];
            face[lvl_c][x_c] = temp;

            temp = face[y][lvl_c];
            face[y][lvl_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

void x_axis_cw90(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, uint16_t i_end) {
    color_t *temp = malloc(sizeof(color_t) * layers);
    bool_t reached;
    for (reached = false; !reached; i += i_step, j += j_step) {
        col2row(cube->front, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->bottom, 0, i, SOUTH, cube->front, 0, i, SOUTH, layers);
        col2col(cube->back, layers - 1, j, NORTH, cube->bottom, 0, i, SOUTH, layers);
        col2col(cube->top, 0, i, SOUTH, cube->back, layers - 1, j, NORTH, layers);
        row2col(temp, 0, 0, EAST, cube->top, 0, i, SOUTH, layers);

        reached = (i == i_end ? true : false);
    }
    free(temp);
}

void x_axis_ccw90(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void x_axis_180(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void y_axis_cw90(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void y_axis_ccw90(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void y_axis_180(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void z_axis_cw90(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void z_axis_ccw90(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void z_axis_180(cube_t *cube, uint16_t layers, uint16_t i, uint16_t j, int16_t i_step,
        int16_t j_step, int16_t i_end) {
}

void move_right_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    uint16_t layers = cube->layers;
    if (start_l == 0)
        rotate_face_cw90(cube->right, layers);
    if (end_l == layers - 1)
        rotate_face_ccw90(cube->left, layers);

    uint16_t i = layers - 1 - start_l, i_end = layers - 1 - end_l, j = start_l;
    int16_t i_step = -1, j_step = 1;
    x_axis_cw90(cube, layers, i, j, i_step, j_step, i_end);
}

void move_right_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_ccw90(cube->right, layers);
    if (end_l == layers - 1)
        rotate_face_cw90(cube->left, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = layers - 1 - start_l, j = start_l; i >= layers - 1 - end_l; i--, j++) {
        col2row(cube->front, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->top, 0, i, SOUTH, cube->front, 0, i, SOUTH, layers);
        col2col(cube->back, layers - 1, j, NORTH, cube->top, 0, i, SOUTH, layers);
        col2col(cube->bottom, 0, i, SOUTH, cube->back, layers - 1, j, NORTH, layers);
        row2col(temp, 0, 0, EAST, cube->bottom, 0, i, SOUTH, layers);
    }
    free(temp);
}

void move_right_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_180(cube->right, layers);
    if (end_l == layers - 1)
        rotate_face_180(cube->left, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = layers - 1 - start_l, j = start_l; i >= layers - 1 - end_l; i--, j++) {
        col2row(cube->top, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->bottom, 0, i, SOUTH, cube->top, 0, i, SOUTH, layers);
        row2col(temp, 0, 0, EAST, cube->bottom, 0, i, SOUTH, layers);

        col2row(cube->front, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->back, layers - 1, j, NORTH, cube->front, 0, i, SOUTH, layers);
        row2col(temp, 0, 0, EAST, cube->back, layers - 1, j, NORTH, layers);
    }
    free(temp);
}

void move_left_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_cw90(cube->left, layers);
    if (end_l == layers - 1)
        rotate_face_ccw90(cube->right, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = start_l, j = layers - 1 - start_l; i <= end_l; i++, j--) {
        col2row(cube->front, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->top, 0, i, SOUTH, cube->front, 0, i, SOUTH, layers);
        col2col(cube->back, layers - 1, j, NORTH, cube->top, 0, i, SOUTH, layers);
        col2col(cube->bottom, 0, i, SOUTH, cube->back, layers - 1, j, NORTH, layers);
        row2col(temp, 0, 0, EAST, cube->bottom, 0, i, SOUTH, layers);
    }
    free(temp);
}

void move_left_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    uint16_t layers = cube->layers;
    if (start_l == 0)
        rotate_face_ccw90(cube->left, layers);
    if (end_l == layers - 1)
        rotate_face_cw90(cube->right, layers);

    uint16_t i = start_l, i_end = end_l, j = layers - 1 - start_l;
    int i_step = 1, j_step = -1;
    x_axis_cw90(cube, layers, i, j, i_step, j_step, i_end);
}

void move_left_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_180(cube->left, layers);
    if (end_l == layers - 1)
        rotate_face_180(cube->right, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = start_l, j = layers - 1 - start_l; i <= end_l; i++, j--) {
        col2row(cube->top, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->bottom, 0, i, SOUTH, cube->top, 0, i, SOUTH, layers);
        row2col(temp, 0, 0, EAST, cube->bottom, 0, i, SOUTH, layers);

        col2row(cube->front, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->back, layers - 1, j, NORTH, cube->front, 0, i, SOUTH, layers);
        row2col(temp, 0, 0, EAST, cube->back, layers - 1, j, NORTH, layers);
    }
    free(temp);
}

void move_front_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_cw90(cube->front, layers);
    if (end_l == layers - 1)
        rotate_face_ccw90(cube->back, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = layers - 1 - start_l, j = start_l; i >= layers - 1 - end_l; i--, j++) {
        row2row(cube->top, i, 0, EAST, temp, 0, 0, EAST, layers);
        col2row(cube->left, layers - 1, i, NORTH, cube->top, i, 0, EAST, layers);
        row2col(cube->bottom, j, layers - 1, WEST, cube->left, layers - 1, i, NORTH, layers);
        col2row(cube->right, 0, j, SOUTH, cube->bottom, j, layers - 1, WEST, layers);
        row2col(temp, 0, 0, EAST, cube->right, 0, j, SOUTH, layers);
    }
    free(temp);
}

void move_front_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_ccw90(cube->front, layers);
    if (end_l == layers - 1)
        rotate_face_cw90(cube->back, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = layers - 1 - start_l, j = start_l; i >= layers - 1 - end_l; i--, j++) {
        row2row(cube->top, i, 0, EAST, temp, 0, 0, EAST, layers);
        col2row(cube->right, 0, j, SOUTH, cube->top, i, 0, EAST, layers);
        row2col(cube->bottom, j, layers - 1, WEST, cube->right, 0, j, SOUTH, layers);
        col2row(cube->left, layers - 1, i, NORTH, cube->bottom, j, layers - 1, WEST, layers);
        row2col(temp, 0, 0, EAST, cube->left, layers - 1, i, NORTH, layers);
    }
    free(temp);
}

void move_front_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_180(cube->front, layers);
    if (end_l == layers - 1)
        rotate_face_180(cube->back, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = layers - 1 - start_l, j = start_l; i >= layers - 1 - end_l; i--, j++) {
        row2row(cube->top, i, 0, EAST, temp, 0, 0, EAST, layers);
        row2row(cube->bottom, j, layers - 1, WEST, cube->top, i, 0, EAST, layers);
        row2row(temp, 0, 0, EAST, cube->bottom, j, layers - 1, WEST, layers);

        col2row(cube->right, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->left, layers - 1, j, NORTH, cube->right, 0, i, SOUTH, layers);
        row2col(temp, 0, 0, EAST, cube->left, layers - 1, j, NORTH, layers);
    }
    free(temp);
}

void move_back_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_cw90(cube->back, layers);
    if (end_l == layers - 1)
        rotate_face_ccw90(cube->front, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = start_l, j = layers - 1 - start_l; i <= end_l; i++, j--) {
        row2row(cube->top, i, 0, EAST, temp, 0, 0, EAST, layers);
        col2row(cube->right, 0, j, SOUTH, cube->top, i, 0, EAST, layers);
        row2col(cube->bottom, j, layers - 1, WEST, cube->right, 0, j, SOUTH, layers);
        col2row(cube->left, layers - 1, i, NORTH, cube->bottom, j, layers - 1, WEST, layers);
        row2col(temp, 0, 0, EAST, cube->left, layers - 1, i, NORTH, layers);
    }
    free(temp);
}

void move_back_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_ccw90(cube->back, layers);
    if (end_l == layers - 1)
        rotate_face_cw90(cube->front, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = start_l, j = layers - 1 - start_l; i <= end_l; i++, j--) {
        row2row(cube->top, i, 0, EAST, temp, 0, 0, EAST, layers);
        col2row(cube->left, layers - 1, i, NORTH, cube->top, i, 0, EAST, layers);
        row2col(cube->bottom, j, layers - 1, WEST, cube->left, layers - 1, i, NORTH, layers);
        col2row(cube->right, 0, j, SOUTH, cube->bottom, j, layers - 1, WEST, layers);
        row2col(temp, 0, 0, EAST, cube->right, 0, j, SOUTH, layers);
    }
    free(temp);
}

void move_back_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {
    int layers = cube->layers, i, j;

    if (start_l == 0)
        rotate_face_180(cube->front, layers);
    if (end_l == layers - 1)
        rotate_face_180(cube->back, layers);

    color_t *temp = malloc(sizeof(color_t) * layers);
    for (i = start_l, j = layers - 1 - start_l; i <= end_l; i++, j--) {
        row2row(cube->top, i, 0, EAST, temp, 0, 0, EAST, layers);
        row2row(cube->bottom, j, layers - 1, WEST, cube->top, i, 0, EAST, layers);
        row2row(temp, 0, 0, EAST, cube->bottom, j, layers - 1, WEST, layers);

        col2row(cube->right, 0, i, SOUTH, temp, 0, 0, EAST, layers);
        col2col(cube->left, layers - 1, j, NORTH, cube->right, 0, i, SOUTH, layers);
        row2col(temp, 0, 0, EAST, cube->left, layers - 1, j, NORTH, layers);
    }
    free(temp);
}

void move_top_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {

}

void move_top_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {

}

void move_top_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {

}

void move_bottom_cw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {

}

void move_bottom_ccw90(cube_t *cube, uint16_t start_l, uint16_t end_l) {

}

void move_bottom_180(cube_t *cube, uint16_t start_l, uint16_t end_l) {

}

typedef void (*cube_move)(cube_t *, uint16_t, uint16_t);

static cube_move op_list[TOTAL_OPS] = { &move_right_cw90, &move_right_ccw90, &move_right_180,
        &move_left_cw90, &move_left_ccw90, &move_left_180, &move_top_cw90, &move_top_ccw90,
        &move_top_180, &move_bottom_cw90, &move_bottom_ccw90, &move_bottom_180, &move_front_cw90,
        &move_front_ccw90, &move_front_180, &move_back_cw90, &move_back_ccw90, &move_back_180 };

void move(cube_t *cube, step_t step, uint16_t start_l, uint16_t end_l) {
    if (start_l > end_l)
        SWAP(start_l, end_l);
    end_l = MIN(end_l, cube->layers - 1);

    op_list[step](cube, start_l, end_l);
}
