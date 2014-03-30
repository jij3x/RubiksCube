#include "rubiks_cube.h"

cube_t *create_cube(lyrnum_t layers) {
    layers = fmin(layers, MAX_LAYERS);
    layers = fmax(layers, MIN_LAYERS);
    lyrnum_t raw_face_size = pow(layers, 2) * sizeof(color_t) + ALIGNMENT - 1;

    color_t **raw_face_pool = malloc(sizeof(uintptr_t) * TOTAL_COLORS);
    color_t **face_pool = malloc(sizeof(uintptr_t) * TOTAL_COLORS);
    for (color_t i; i < TOTAL_COLORS; i++) {
        color_t *raw_face = malloc(raw_face_size);
        if (raw_face == NULL)
            abort();
        memset(raw_face, i, raw_face_size);

        uintptr_t aligned_addr = ~((uintptr_t) ALIGNMENT - 1);
        aligned_addr &= ((uintptr_t) raw_face + ALIGNMENT - 1);
        color_t *face = (color_t *) aligned_addr;

        face_pool[i] = face;
        raw_face_pool[i] = raw_face;
    }

    cube_t init_cube = { .layers = layers, .raw_face_size = raw_face_size, .faces = face_pool,
            .raw_faces = raw_face_pool };
    cube_t *cube = malloc(sizeof(cube_t));
    if (cube == NULL)
        abort();
    memcpy(cube, &init_cube, sizeof(cube_t));

    for (color_t i; i < TOTAL_COLORS; i++) {
        cube->hands[i] = cube->faces[i];
        cube->hand_idx[i] = i;
    }

    return cube;
}

void destroy_cube(cube_t *cube) {
    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        free(cube->raw_faces[i]);
    }

    free(cube);
}

cube_t *clone_cube(cube_t *cube) {
    cube_t *clone = create_cube(cube->layers);

    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        memcpy(clone->raw_faces[i], cube->raw_faces[i], clone->raw_face_size);

        clone->hand_idx[i] = cube->hand_idx[i];
        clone->hands[i] = clone->faces[clone->hand_idx[i]];
    }

    return clone;
}

static inline void rotate_face_90_cw(color_t *face_ptr, lyrnum_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (lyrnum_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (lyrnum_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[y_c][lvl];
            face[y_c][lvl] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y][lvl_c];
            face[y][lvl_c] = temp;
        }
    }
}

static inline void rotate_face_90_c_cw(color_t *face_ptr, lyrnum_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (lyrnum_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (lyrnum_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[y][lvl_c];
            face[y][lvl_c] = face[lvl_c][x_c];
            face[lvl_c][x_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

void rotate_face_180(color_t *face_ptr, lyrnum_t layers) {
    assert(layers >= MIN_LAYERS && layers <= MAX_LAYERS);

    color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;
    color_t temp;
    for (lyrnum_t lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
        for (lyrnum_t x = lvl, y = x, x_c = lvl_c, y_c = x_c; x < lvl_c; x++, y++, x_c--, y_c--) {
            temp = face[lvl][x];
            face[lvl][x] = face[lvl_c][x_c];
            face[lvl_c][x_c] = temp;

            temp = face[y][lvl_c];
            face[y][lvl_c] = face[y_c][lvl];
            face[y_c][lvl] = temp;
        }
    }
}

static inline void normalize_layers(cube_t *cube, lyrnum_t *start_l, lyrnum_t *end_l) {
    if (*start_l > *end_l)
        SWAP(*start_l, *end_l);

    *start_l = fmin(cube->layers - 1, *start_l);
    *end_l = fmin(cube->layers - 1, *end_l);
}

void rotate_x_90_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x) {
    normalize_layers(cube, &start_x, &end_x);

    lyrnum_t layers = cube->layers;
    color_t (*yellow)[layers] = (color_t (*)[layers]) cube->faces[YELLOW];
    color_t (*white)[layers] = (color_t (*)[layers]) cube->faces[WHITE];
    color_t (*blue)[layers] = (color_t (*)[layers]) cube->faces[BLUE];
    color_t (*green)[layers] = (color_t (*)[layers]) cube->faces[GREEN];

    color_t temp;
    for (lyrnum_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = blue[j][i];
            blue[j][i] = white[j][i];
            white[j][i] = green[j_c][i_c];
            green[j_c][i_c] = yellow[j][i];
            yellow[j][i] = temp;
        }
    }

    if (start_x == 0)
        rotate_face_90_c_cw(cube->faces[ORANGE], layers);
    if (end_x == layers - 1)
        rotate_face_90_cw(cube->faces[RED], layers);
}

void rotate_x_90_c_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x) {
    normalize_layers(cube, &start_x, &end_x);

    lyrnum_t layers = cube->layers;
    color_t (*yellow)[layers] = (color_t (*)[layers]) cube->faces[YELLOW];
    color_t (*white)[layers] = (color_t (*)[layers]) cube->faces[WHITE];
    color_t (*blue)[layers] = (color_t (*)[layers]) cube->faces[BLUE];
    color_t (*green)[layers] = (color_t (*)[layers]) cube->faces[GREEN];

    color_t temp;
    for (lyrnum_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = blue[j][i];
            blue[j][i] = yellow[j][i];
            yellow[j][i] = green[j_c][i_c];
            green[j_c][i_c] = white[j][i];
            white[j][i] = temp;
        }
    }

    if (start_x == 0)
        rotate_face_90_cw(cube->faces[ORANGE], layers);
    if (end_x == layers - 1)
        rotate_face_90_c_cw(cube->faces[RED], layers);
}

void rotate_x_180(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x) {
    normalize_layers(cube, &start_x, &end_x);

    lyrnum_t layers = cube->layers;
    color_t (*yellow)[layers] = (color_t (*)[layers]) cube->faces[YELLOW];
    color_t (*white)[layers] = (color_t (*)[layers]) cube->faces[WHITE];
    color_t (*blue)[layers] = (color_t (*)[layers]) cube->faces[BLUE];
    color_t (*green)[layers] = (color_t (*)[layers]) cube->faces[GREEN];

    color_t temp;
    for (lyrnum_t i = start_x, i_c = end_x; i <= end_x; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = yellow[j][i];
            yellow[j][i] = white[j][i];
            white[j][i] = temp;

            temp = blue[j][i];
            blue[j][i] = green[j_c][i_c];
            green[j_c][i_c] = temp;
        }
    }

    if (start_x == 0)
        rotate_face_180(cube->faces[ORANGE], layers);
    if (end_x == layers - 1)
        rotate_face_180(cube->faces[RED], layers);
}

void rotate_y_90_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y) {
    normalize_layers(cube, &start_y, &end_y);

    lyrnum_t layers = cube->layers;
    color_t (*blue)[layers] = (color_t (*)[layers]) cube->faces[BLUE];
    color_t (*green)[layers] = (color_t (*)[layers]) cube->faces[GREEN];
    color_t (*orange)[layers] = (color_t (*)[layers]) cube->faces[ORANGE];
    color_t (*red)[layers] = (color_t (*)[layers]) cube->faces[RED];

    color_t temp;
    for (lyrnum_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = blue[i][j];
            blue[i][j] = red[i][j];
            red[i][j] = green[i][j];
            green[i][j] = orange[i][j];
            orange[i][j] = temp;
        }
    }

    if (start_y == 0)
        rotate_face_90_cw(cube->faces[YELLOW], layers);
    if (end_y == layers - 1)
        rotate_face_90_c_cw(cube->faces[WHITE], layers);
}

void rotate_y_90_c_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y) {
    normalize_layers(cube, &start_y, &end_y);

    lyrnum_t layers = cube->layers;
    color_t (*blue)[layers] = (color_t (*)[layers]) cube->faces[BLUE];
    color_t (*green)[layers] = (color_t (*)[layers]) cube->faces[GREEN];
    color_t (*orange)[layers] = (color_t (*)[layers]) cube->faces[ORANGE];
    color_t (*red)[layers] = (color_t (*)[layers]) cube->faces[RED];

    color_t temp;
    for (lyrnum_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = blue[i][j];
            blue[i][j] = orange[i][j];
            orange[i][j] = green[i][j];
            green[i][j] = red[i][j];
            red[i][j] = temp;
        }
    }

    if (start_y == 0)
        rotate_face_90_c_cw(cube->faces[YELLOW], layers);
    if (end_y == layers - 1)
        rotate_face_90_cw(cube->faces[WHITE], layers);
}

void rotate_y_180(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y) {
    normalize_layers(cube, &start_y, &end_y);

    lyrnum_t layers = cube->layers;
    color_t (*blue)[layers] = (color_t (*)[layers]) cube->faces[BLUE];
    color_t (*green)[layers] = (color_t (*)[layers]) cube->faces[GREEN];
    color_t (*orange)[layers] = (color_t (*)[layers]) cube->faces[ORANGE];
    color_t (*red)[layers] = (color_t (*)[layers]) cube->faces[RED];

    color_t temp;
    for (lyrnum_t i = start_y, i_c = end_y; i <= end_y; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = blue[i][j];
            blue[i][j] = green[i][j];
            green[i][j] = temp;

            temp = orange[i][j];
            orange[i][j] = red[i][j];
            red[i][j] = temp;
        }
    }

    if (start_y == 0)
        rotate_face_180(cube->faces[YELLOW], layers);
    if (end_y == layers - 1)
        rotate_face_180(cube->faces[WHITE], layers);
}

void rotate_z_90_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z) {
    normalize_layers(cube, &start_z, &end_z);

    lyrnum_t layers = cube->layers;
    color_t (*orange)[layers] = (color_t (*)[layers]) cube->faces[ORANGE];
    color_t (*red)[layers] = (color_t (*)[layers]) cube->faces[RED];
    color_t (*yellow)[layers] = (color_t (*)[layers]) cube->faces[YELLOW];
    color_t (*white)[layers] = (color_t (*)[layers]) cube->faces[WHITE];

    color_t temp;
    for (lyrnum_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = orange[j][i];
            orange[j][i] = white[i_c][j];
            white[i_c][j] = red[j_c][i_c];
            red[j_c][i_c] = yellow[i][j_c];
            yellow[i][j_c] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_90_c_cw(cube->faces[GREEN], layers);
    if (end_z == layers - 1)
        rotate_face_90_cw(cube->faces[BLUE], layers);
}

void rotate_z_90_c_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z) {
    normalize_layers(cube, &start_z, &end_z);

    lyrnum_t layers = cube->layers;
    color_t (*orange)[layers] = (color_t (*)[layers]) cube->faces[ORANGE];
    color_t (*red)[layers] = (color_t (*)[layers]) cube->faces[RED];
    color_t (*yellow)[layers] = (color_t (*)[layers]) cube->faces[YELLOW];
    color_t (*white)[layers] = (color_t (*)[layers]) cube->faces[WHITE];

    color_t temp;
    for (lyrnum_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = orange[j][i];
            orange[j][i] = yellow[i][j_c];
            yellow[i][j_c] = red[j_c][i_c];
            red[j_c][i_c] = white[i_c][j];
            white[i_c][j] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_90_cw(cube->faces[BLUE], layers);
    if (end_z == layers - 1)
        rotate_face_90_c_cw(cube->faces[GREEN], layers);
}

void rotate_z_180(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z) {
    normalize_layers(cube, &start_z, &end_z);

    lyrnum_t layers = cube->layers;
    color_t (*orange)[layers] = (color_t (*)[layers]) cube->faces[ORANGE];
    color_t (*red)[layers] = (color_t (*)[layers]) cube->faces[RED];
    color_t (*yellow)[layers] = (color_t (*)[layers]) cube->faces[YELLOW];
    color_t (*white)[layers] = (color_t (*)[layers]) cube->faces[WHITE];

    color_t temp;
    for (lyrnum_t i = start_z, i_c = end_z; i < end_z; i++, i_c--) {
        for (lyrnum_t j = 0, j_c = layers - 1; j < layers; j++, j_c--) {
            temp = orange[j][i];
            orange[j][i] = red[j_c][i_c];
            red[j_c][i_c] = temp;

            temp = yellow[i][j_c];
            yellow[i][j_c] = white[i_c][j];
            white[i_c][j] = temp;
        }
    }

    if (start_z == 0)
        rotate_face_180(cube->faces[GREEN], layers);
    if (end_z == layers - 1)
        rotate_face_180(cube->faces[BLUE], layers);
}

typedef void (*rotate)(cube_t *, lyrnum_t, lyrnum_t);
static const rotate rotates[TOTAL_COLORS * TOTAL_ROTATIONS] = {
// relative to absolute rotation mapping

        NULL,// YELLOW : NO_ROTATION
        &rotate_y_90_c_cw, // YELLOW : R_90_CW
        &rotate_y_90_cw, // YELLOW : R_90_C_CW
        &rotate_y_180, // YELLOW : R_180
        NULL, // WHITE : NO_ROTATION
        &rotate_y_90_cw, // WHITE : R_90_CW
        &rotate_y_90_c_cw, // WHITE : R_90_C_CW
        &rotate_y_180, // WHITE : R_180
        NULL, // RED : NO_ROTATION
        &rotate_x_90_cw, // RED : R_90_CW
        &rotate_x_90_c_cw, // RED : R_90_C_CW
        &rotate_x_180, // RED : R_180
        NULL, // ORANGE : NO_ROTATION
        &rotate_x_90_c_cw, // ORANGE : R_90_CW
        &rotate_x_90_cw, // ORANGE : R_90_C_CW
        &rotate_x_180, // ORANGE : R_180
        NULL, // BLUE : NO_ROTATION
        &rotate_z_90_cw, // BLUE : R_90_CW
        &rotate_z_90_c_cw, // BLUE : R_90_C_CW
        &rotate_z_180, // BLUE : R_180
        NULL, // GREEN : NO_ROTATION
        &rotate_z_90_c_cw, // GREEN : R_90_CW
        &rotate_z_90_cw, // GREEN : R_90_C_CW
        &rotate_z_180 // GREEN : R_180

        };

void move_cube(cube_t *cube, hand_t hand, rotation_t rotation, lyrnum_t start_l, lyrnum_t end_l) {
    if (start_l > end_l)
        SWAP(start_l, end_l);
    start_l = fmin(cube->layers - 1, start_l);
    end_l = fmin(cube->layers - 1, end_l);

    color_t face_color = cube->hand_idx[hand];
    if (face_color == YELLOW || face_color == RED || face_color == BLUE) {
        start_l = cube->layers - 1 - start_l;
        end_l = cube->layers - 1 - end_l;
        SWAP(start_l, end_l);
    }

    rotates[face_color * TOTAL_ROTATIONS + rotation](cube, start_l, end_l);
}

static inline void cpon_x_90_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[FRONT_HAND];
    cube->hand_idx[FRONT_HAND] = cube->hand_idx[BOTTOM_HAND];
    cube->hand_idx[BOTTOM_HAND] = cube->hand_idx[BACK_HAND];
    cube->hand_idx[BACK_HAND] = cube->hand_idx[TOP_HAND];
    cube->hand_idx[TOP_HAND] = temp_idx;
}

static inline void cpon_x_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[FRONT_HAND];
    cube->hand_idx[FRONT_HAND] = cube->hand_idx[TOP_HAND];
    cube->hand_idx[TOP_HAND] = cube->hand_idx[BACK_HAND];
    cube->hand_idx[BACK_HAND] = cube->hand_idx[BOTTOM_HAND];
    cube->hand_idx[BOTTOM_HAND] = temp_idx;
}

static inline void cpon_x_180(cube_t * cube) {
    color_t *temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[FRONT_HAND];
    cube->hand_idx[FRONT_HAND] = cube->hand_idx[BACK_HAND];
    cube->hand_idx[BACK_HAND] = temp_idx;

    temp_face = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;

    temp_idx = cube->hand_idx[TOP_HAND];
    cube->hand_idx[TOP_HAND] = cube->hand_idx[BOTTOM_HAND];
    cube->hand_idx[BOTTOM_HAND] = temp_idx;
}

static inline void cpon_y_90_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[RIGHT_HAND];
    cube->hand_idx[RIGHT_HAND] = cube->hand_idx[FRONT_HAND];
    cube->hand_idx[FRONT_HAND] = cube->hand_idx[LEFT_HAND];
    cube->hand_idx[LEFT_HAND] = cube->hand_idx[BACK_HAND];
    cube->hand_idx[BACK_HAND] = temp_idx;
}

static inline void cpon_y_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[RIGHT_HAND];
    cube->hand_idx[RIGHT_HAND] = cube->hand_idx[BACK_HAND];
    cube->hand_idx[BACK_HAND] = cube->hand_idx[LEFT_HAND];
    cube->hand_idx[LEFT_HAND] = cube->hand_idx[FRONT_HAND];
    cube->hand_idx[FRONT_HAND] = temp_idx;
}

static inline void cpon_y_180(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[RIGHT_HAND];
    cube->hand_idx[RIGHT_HAND] = cube->hand_idx[LEFT_HAND];
    cube->hand_idx[LEFT_HAND] = temp_idx;

    temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = temp_face;

    temp_idx = cube->hand_idx[FRONT_HAND];
    cube->hand_idx[FRONT_HAND] = cube->hand_idx[BACK_HAND];
    cube->hand_idx[BACK_HAND] = temp_idx;
}

static inline void cpon_z_90_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[RIGHT_HAND];
    cube->hand_idx[RIGHT_HAND] = cube->hand_idx[TOP_HAND];
    cube->hand_idx[TOP_HAND] = cube->hand_idx[LEFT_HAND];
    cube->hand_idx[LEFT_HAND] = cube->hand_idx[BOTTOM_HAND];
    cube->hand_idx[BOTTOM_HAND] = temp_idx;
}

static inline void cpon_z_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[RIGHT_HAND];
    cube->hand_idx[RIGHT_HAND] = cube->hand_idx[BOTTOM_HAND];
    cube->hand_idx[BOTTOM_HAND] = cube->hand_idx[LEFT_HAND];
    cube->hand_idx[LEFT_HAND] = cube->hand_idx[TOP_HAND];
    cube->hand_idx[TOP_HAND] = temp_idx;
}

static inline void cpon_z_180(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = temp_face;

    color_t temp_idx = cube->hand_idx[RIGHT_HAND];
    cube->hand_idx[RIGHT_HAND] = cube->hand_idx[LEFT_HAND];
    cube->hand_idx[LEFT_HAND] = temp_idx;

    temp_face = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;

    temp_idx = cube->hand_idx[TOP_HAND];
    cube->hand_idx[TOP_HAND] = cube->hand_idx[BOTTOM_HAND];
    cube->hand_idx[BOTTOM_HAND] = temp_idx;
}

void flip_cube(cube_t *cube, axis_t axis, rotation_t rotation) {
    assert(axis < TOTAL_AXIS && rotation < TOTAL_ROTATIONS);

    if (axis == AXIS_X) {
        if (rotation == R_90_CW)
            cpon_x_90_cw(cube);
        else if (rotation == R_90_C_CW)
            cpon_x_90_c_cw(cube);
        else if (rotation == R_180)
            cpon_x_180(cube);
    } else if (axis == AXIS_Y) {
        if (rotation == R_90_CW)
            cpon_y_90_cw(cube);
        else if (rotation == R_90_C_CW)
            cpon_y_90_c_cw(cube);
        else if (rotation == R_180)
            cpon_y_180(cube);
    } else {
        if (rotation == R_90_CW)
            cpon_z_90_cw(cube);
        else if (rotation == R_90_C_CW)
            cpon_z_90_c_cw(cube);
        else if (rotation == R_180)
            cpon_z_180(cube);
    }
}

void reset_coordinate(cube_t *cube) {
    switch (cube->hand_idx[TOP_HAND]) {
    case WHITE:
        flip_cube(cube, AXIS_Z, R_180);
        break;
    case RED:
        flip_cube(cube, AXIS_Z, R_90_CW);
        break;
    case ORANGE:
        flip_cube(cube, AXIS_Z, R_90_C_CW);
        break;
    case BLUE:
        flip_cube(cube, AXIS_X, R_90_C_CW);
        break;
    case GREEN:
        flip_cube(cube, AXIS_X, R_90_CW);
        break;
    default:
        break;
    }

    switch (cube->hand_idx[RIGHT_HAND]) {
    case WHITE:
        flip_cube(cube, AXIS_Z, R_90_CW);
        break;
    case YELLOW:
        flip_cube(cube, AXIS_Z, R_90_C_CW);
        break;
    case ORANGE:
        flip_cube(cube, AXIS_Y, R_180);
        break;
    case BLUE:
        flip_cube(cube, AXIS_Y, R_90_CW);
        break;
    case GREEN:
        flip_cube(cube, AXIS_Y, R_90_C_CW);
        break;
    default:
        break;
    }

    switch (cube->hand_idx[FRONT_HAND]) {
    case WHITE:
        flip_cube(cube, AXIS_X, R_90_C_CW);
        break;
    case YELLOW:
        flip_cube(cube, AXIS_X, R_90_CW);
        break;
    case RED:
        flip_cube(cube, AXIS_Y, R_90_CW);
        break;
    case ORANGE:
        flip_cube(cube, AXIS_Y, R_90_C_CW);
        break;
    case GREEN:
        flip_cube(cube, AXIS_Y, R_180);
        break;
    default:
        break;
    }
}
