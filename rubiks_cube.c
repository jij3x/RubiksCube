#include "rubiks_cube.h"

cube_t *create_cube(lyrnum_t layers) {
    layers = fmin(layers, MAX_LAYERS);
    layers = fmax(layers, MIN_LAYERS);
    lyrnum_t raw_face_size = pow(layers, 2) * sizeof(color_t) + ALIGNMENT - 1;

    color_t **raw_face_pool = malloc(sizeof(uintptr_t) * TOTAL_COLORS);
    color_t **face_pool = malloc(sizeof(uintptr_t) * TOTAL_COLORS);
    for (color_t i = 0; i < TOTAL_COLORS; i++) {
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

    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        cube->face_offset[i] = NO_ROTATION;
        cube->hands[i] = cube->faces[i];
        cube->hand_color[(hand_t) i] = i;
    }

    return cube;
}

void destroy_cube(cube_t *cube) {
    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        free(cube->raw_faces[i]);
    }

    free(cube);
    cube = NULL;
}

cube_t *clone_cube(cube_t *cube) {
    cube_t *clone = create_cube(cube->layers);

    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        memcpy(clone->raw_faces[i], cube->raw_faces[i], clone->raw_face_size);

        clone->face_offset[i] = cube->face_offset[i];
        clone->hand_color[i] = cube->hand_color[i];
        clone->hands[(hand_t) i] = clone->faces[clone->hand_color[i]];
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

static inline void rotate_face_180(color_t *face_ptr, lyrnum_t layers) {
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

void turn_x_90_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x) {
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

void turn_x_90_c_cw(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x) {
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

void turn_x_180(cube_t *cube, lyrnum_t start_x, lyrnum_t end_x) {
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

void turn_y_90_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y) {
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

void turn_y_90_c_cw(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y) {
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

void turn_y_180(cube_t *cube, lyrnum_t start_y, lyrnum_t end_y) {
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

void turn_z_90_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z) {
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

void turn_z_90_c_cw(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z) {
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

void turn_z_180(cube_t *cube, lyrnum_t start_z, lyrnum_t end_z) {
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

static const rotation_t ra_tbl[TOTAL_ROTATIONS][TOTAL_ROTATIONS] = {
// rotation add-on table
        { NO_ROTATION, R_90_CW, R_90_C_CW, R_180 }, // NO_ROTATION + (NR, R90CW, R90CCW, R180)
        { R_90_CW, R_180, NO_ROTATION, R_90_C_CW }, // R_90_CW + (NR, R90CW, R90CCW, R180)
        { R_90_C_CW, NO_ROTATION, R_180, R_90_CW }, // R_90_C_CW + (NR, R90CW, R90CCW, R180)
        { R_180, R_90_C_CW, R_90_CW, NO_ROTATION }  // R_180 + (NR, R90CW, R90CCW, R180)
};

void rotate_x_90_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = temp_face;

    color_t temp_idx = cube->hand_color[FRONT_HAND];
    cube->hand_color[FRONT_HAND] = cube->hand_color[BOTTOM_HAND];
    cube->hand_color[BOTTOM_HAND] = cube->hand_color[BACK_HAND];
    cube->hand_color[BACK_HAND] = cube->hand_color[TOP_HAND];
    cube->hand_color[TOP_HAND] = temp_idx;

    color_t righthand_c = cube->hand_color[RIGHT_HAND];
    cube->face_offset[righthand_c] = ra_tbl[cube->face_offset[righthand_c]][R_90_CW];
    color_t lefthand_c = cube->hand_color[LEFT_HAND];
    cube->face_offset[lefthand_c] = ra_tbl[cube->face_offset[lefthand_c]][R_90_C_CW];
    color_t backhand_c = cube->hand_color[BACK_HAND];
    cube->face_offset[backhand_c] = ra_tbl[cube->face_offset[backhand_c]][R_180];
    color_t bottomhand_c = cube->hand_color[BOTTOM_HAND];
    cube->face_offset[bottomhand_c] = ra_tbl[cube->face_offset[bottomhand_c]][R_180];
}

void rotate_x_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;

    color_t temp_idx = cube->hand_color[FRONT_HAND];
    cube->hand_color[FRONT_HAND] = cube->hand_color[TOP_HAND];
    cube->hand_color[TOP_HAND] = cube->hand_color[BACK_HAND];
    cube->hand_color[BACK_HAND] = cube->hand_color[BOTTOM_HAND];
    cube->hand_color[BOTTOM_HAND] = temp_idx;

    color_t righthand_c = cube->hand_color[RIGHT_HAND];
    cube->face_offset[righthand_c] = ra_tbl[cube->face_offset[righthand_c]][R_90_C_CW];
    color_t lefthand_c = cube->hand_color[LEFT_HAND];
    cube->face_offset[lefthand_c] = ra_tbl[cube->face_offset[lefthand_c]][R_90_CW];
    color_t backhand_c = cube->hand_color[BACK_HAND];
    cube->face_offset[backhand_c] = ra_tbl[cube->face_offset[backhand_c]][R_180];
    color_t tophand_c = cube->hand_color[TOP_HAND];
    cube->face_offset[tophand_c] = ra_tbl[cube->face_offset[tophand_c]][R_180];
}

void rotate_x_180(cube_t * cube) {
    color_t *temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = temp_face;
    color_t temp_idx = cube->hand_color[FRONT_HAND];
    cube->hand_color[FRONT_HAND] = cube->hand_color[BACK_HAND];
    cube->hand_color[BACK_HAND] = temp_idx;

    temp_face = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;
    temp_idx = cube->hand_color[TOP_HAND];
    cube->hand_color[TOP_HAND] = cube->hand_color[BOTTOM_HAND];
    cube->hand_color[BOTTOM_HAND] = temp_idx;

    color_t righthand_c = cube->hand_color[RIGHT_HAND];
    cube->face_offset[righthand_c] = ra_tbl[cube->face_offset[righthand_c]][R_180];
    color_t lefthand_c = cube->hand_color[LEFT_HAND];
    cube->face_offset[lefthand_c] = ra_tbl[cube->face_offset[lefthand_c]][R_180];
    color_t fronthand_c = cube->hand_color[FRONT_HAND];
    cube->face_offset[fronthand_c] = ra_tbl[cube->face_offset[fronthand_c]][R_180];
    color_t backhand_c = cube->hand_color[BACK_HAND];
    cube->face_offset[backhand_c] = ra_tbl[cube->face_offset[backhand_c]][R_180];
}

void rotate_y_90_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = temp_face;

    color_t temp_idx = cube->hand_color[RIGHT_HAND];
    cube->hand_color[RIGHT_HAND] = cube->hand_color[FRONT_HAND];
    cube->hand_color[FRONT_HAND] = cube->hand_color[LEFT_HAND];
    cube->hand_color[LEFT_HAND] = cube->hand_color[BACK_HAND];
    cube->hand_color[BACK_HAND] = temp_idx;

    color_t tophand_c = cube->hand_color[TOP_HAND];
    cube->face_offset[tophand_c] = ra_tbl[cube->face_offset[tophand_c]][R_90_C_CW];
    color_t bottomhand_c = cube->hand_color[BOTTOM_HAND];
    cube->face_offset[bottomhand_c] = ra_tbl[cube->face_offset[bottomhand_c]][R_90_CW];
}

void rotate_y_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = temp_face;

    color_t temp_idx = cube->hand_color[RIGHT_HAND];
    cube->hand_color[RIGHT_HAND] = cube->hand_color[BACK_HAND];
    cube->hand_color[BACK_HAND] = cube->hand_color[LEFT_HAND];
    cube->hand_color[LEFT_HAND] = cube->hand_color[FRONT_HAND];
    cube->hand_color[FRONT_HAND] = temp_idx;

    color_t tophand_c = cube->hand_color[TOP_HAND];
    cube->face_offset[tophand_c] = ra_tbl[cube->face_offset[tophand_c]][R_90_CW];
    color_t bottomhand_c = cube->hand_color[BOTTOM_HAND];
    cube->face_offset[bottomhand_c] = ra_tbl[cube->face_offset[bottomhand_c]][R_90_C_CW];
}

void rotate_y_180(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = temp_face;
    color_t temp_idx = cube->hand_color[RIGHT_HAND];
    cube->hand_color[RIGHT_HAND] = cube->hand_color[LEFT_HAND];
    cube->hand_color[LEFT_HAND] = temp_idx;

    temp_face = cube->hands[FRONT_HAND];
    cube->hands[FRONT_HAND] = cube->hands[BACK_HAND];
    cube->hands[BACK_HAND] = temp_face;
    temp_idx = cube->hand_color[FRONT_HAND];
    cube->hand_color[FRONT_HAND] = cube->hand_color[BACK_HAND];
    cube->hand_color[BACK_HAND] = temp_idx;

    color_t tophand_c = cube->hand_color[TOP_HAND];
    cube->face_offset[tophand_c] = ra_tbl[cube->face_offset[tophand_c]][R_180];
    color_t bottomhand_c = cube->hand_color[BOTTOM_HAND];
    cube->face_offset[bottomhand_c] = ra_tbl[cube->face_offset[bottomhand_c]][R_180];
}

void rotate_z_90_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;

    color_t temp_idx = cube->hand_color[RIGHT_HAND];
    cube->hand_color[RIGHT_HAND] = cube->hand_color[TOP_HAND];
    cube->hand_color[TOP_HAND] = cube->hand_color[LEFT_HAND];
    cube->hand_color[LEFT_HAND] = cube->hand_color[BOTTOM_HAND];
    cube->hand_color[BOTTOM_HAND] = temp_idx;

    color_t fronthand_c = cube->hand_color[FRONT_HAND];
    cube->face_offset[fronthand_c] = ra_tbl[cube->face_offset[fronthand_c]][R_90_CW];
    color_t backhand_c = cube->hand_color[BACK_HAND];
    cube->face_offset[backhand_c] = ra_tbl[cube->face_offset[backhand_c]][R_90_C_CW];
    color_t righthand_c = cube->hand_color[RIGHT_HAND];
    cube->face_offset[righthand_c] = ra_tbl[cube->face_offset[righthand_c]][R_90_CW];
    color_t lefthand_c = cube->hand_color[LEFT_HAND];
    cube->face_offset[lefthand_c] = ra_tbl[cube->face_offset[lefthand_c]][R_90_CW];
    color_t tophand_c = cube->hand_color[TOP_HAND];
    cube->face_offset[tophand_c] = ra_tbl[cube->face_offset[tophand_c]][R_90_CW];
    color_t bottomhand_c = cube->hand_color[BOTTOM_HAND];
    cube->face_offset[bottomhand_c] = ra_tbl[cube->face_offset[bottomhand_c]][R_90_CW];
}

void rotate_z_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = temp_face;

    color_t temp_idx = cube->hand_color[RIGHT_HAND];
    cube->hand_color[RIGHT_HAND] = cube->hand_color[BOTTOM_HAND];
    cube->hand_color[BOTTOM_HAND] = cube->hand_color[LEFT_HAND];
    cube->hand_color[LEFT_HAND] = cube->hand_color[TOP_HAND];
    cube->hand_color[TOP_HAND] = temp_idx;

    color_t fronthand_c = cube->hand_color[FRONT_HAND];
    cube->face_offset[fronthand_c] = ra_tbl[cube->face_offset[fronthand_c]][R_90_C_CW];
    color_t backhand_c = cube->hand_color[BACK_HAND];
    cube->face_offset[backhand_c] = ra_tbl[cube->face_offset[backhand_c]][R_90_CW];
    color_t righthand_c = cube->hand_color[RIGHT_HAND];
    cube->face_offset[righthand_c] = ra_tbl[cube->face_offset[righthand_c]][R_90_C_CW];
    color_t lefthand_c = cube->hand_color[LEFT_HAND];
    cube->face_offset[lefthand_c] = ra_tbl[cube->face_offset[lefthand_c]][R_90_C_CW];
    color_t tophand_c = cube->hand_color[TOP_HAND];
    cube->face_offset[tophand_c] = ra_tbl[cube->face_offset[tophand_c]][R_90_C_CW];
    color_t bottomhand_c = cube->hand_color[BOTTOM_HAND];
    cube->face_offset[bottomhand_c] = ra_tbl[cube->face_offset[bottomhand_c]][R_90_C_CW];
}

void rotate_z_180(cube_t * cube) {
    color_t *temp_face = cube->hands[RIGHT_HAND];
    cube->hands[RIGHT_HAND] = cube->hands[LEFT_HAND];
    cube->hands[LEFT_HAND] = temp_face;
    color_t temp_idx = cube->hand_color[RIGHT_HAND];
    cube->hand_color[RIGHT_HAND] = cube->hand_color[LEFT_HAND];
    cube->hand_color[LEFT_HAND] = temp_idx;

    temp_face = cube->hands[TOP_HAND];
    cube->hands[TOP_HAND] = cube->hands[BOTTOM_HAND];
    cube->hands[BOTTOM_HAND] = temp_face;
    temp_idx = cube->hand_color[TOP_HAND];
    cube->hand_color[TOP_HAND] = cube->hand_color[BOTTOM_HAND];
    cube->hand_color[BOTTOM_HAND] = temp_idx;

    color_t fronthand_c = cube->hand_color[FRONT_HAND];
    cube->face_offset[fronthand_c] = ra_tbl[cube->face_offset[fronthand_c]][R_180];
    color_t backhand_c = cube->hand_color[BACK_HAND];
    cube->face_offset[backhand_c] = ra_tbl[cube->face_offset[backhand_c]][R_180];
    color_t righthand_c = cube->hand_color[RIGHT_HAND];
    cube->face_offset[righthand_c] = ra_tbl[cube->face_offset[righthand_c]][R_180];
    color_t lefthand_c = cube->hand_color[LEFT_HAND];
    cube->face_offset[lefthand_c] = ra_tbl[cube->face_offset[lefthand_c]][R_180];
}

typedef void (*turn_func_t)(cube_t *, lyrnum_t, lyrnum_t);
typedef void (*rotate_func_t)(cube_t *); // layers does not matter to rotate functions
typedef union move_func_u {
    turn_func_t turn;
    rotate_func_t rotate;
} move_func_t;

static const move_func_t move_ops[TOTAL_MOVES] = {
// (turn + rotate) functions
        (move_func_t) &turn_y_90_c_cw, // YELLOW hand: R_90_CW
        (move_func_t) &turn_y_90_cw, //   YELLOW hand: R_90_C_CW
        (move_func_t) &turn_y_180, //     YELLOW hand: R_180
        (move_func_t) &turn_y_90_cw, //   WHITE hand: R_90_CW
        (move_func_t) &turn_y_90_c_cw, // WHITE hand: R_90_C_CW
        (move_func_t) &turn_y_180, //     WHITE hand: R_180
        (move_func_t) &turn_x_90_cw, //   RED hand: R_90_CW
        (move_func_t) &turn_x_90_c_cw, // RED hand: R_90_C_CW
        (move_func_t) &turn_x_180, //     RED hand: R_180
        (move_func_t) &turn_x_90_c_cw, // ORANGE hand: R_90_CW
        (move_func_t) &turn_x_90_cw, //   ORANGE hand: R_90_C_CW
        (move_func_t) &turn_x_180, //     ORANGE hand: R_180
        (move_func_t) &turn_z_90_cw, //   BLUE hand: R_90_CW
        (move_func_t) &turn_z_90_c_cw, // BLUE hand: R_90_C_CW
        (move_func_t) &turn_z_180, //     BLUE hand: R_180
        (move_func_t) &turn_z_90_c_cw, // GREEN hand: R_90_CW
        (move_func_t) &turn_z_90_cw, //   GREEN hand: R_90_C_CW
        (move_func_t) &turn_z_180, //     GREEN hand: R_180
        (move_func_t) &rotate_x_90_cw, //   cube rotate : X_R90_CW
        (move_func_t) &rotate_x_90_c_cw, // cube rotate : X_R90_C_CW
        (move_func_t) &rotate_x_180, //     cube rotate : X_R180
        (move_func_t) &rotate_y_90_cw, //   cube rotate : Y_R90_CW
        (move_func_t) &rotate_y_90_c_cw, // cube rotate : Y_R90_C_CW
        (move_func_t) &rotate_y_180, //     cube rotate : Y_R180
        (move_func_t) &rotate_z_90_cw, //   cube rotate : Z_R90_CW
        (move_func_t) &rotate_z_90_c_cw, // cube rotate : Z_R90_C_CW
        (move_func_t) &rotate_z_180 //      cube rotate : Z_R180
        };

void move_cube(cube_t *cube, move_t move, lyrnum_t start_l, lyrnum_t end_l) {
    if (move > BACK_T180) {
        move_ops[move].rotate(cube);
        return;
    }

    if (start_l > end_l)
        SWAP(start_l, end_l);
    start_l = fmin(cube->layers - 1, start_l);
    end_l = fmin(cube->layers - 1, end_l);

    color_t hand_color = cube->hand_color[move / 3];
    if (hand_color == YELLOW || hand_color == RED || hand_color == BLUE) {
        start_l = cube->layers - 1 - start_l;
        end_l = cube->layers - 1 - end_l;
        SWAP(start_l, end_l);
    }

    move_ops[hand_color * 3 + move % 3].turn(cube, start_l, end_l);
}

void reset_coordinate(cube_t *cube) {
    switch (cube->hand_color[TOP_HAND]) {
    case YELLOW:
    case WHITE:
        if (cube->hand_color[TOP_HAND] == WHITE)
            move_cube(cube, X_R180, 0, 0);

        switch (cube->hand_color[RIGHT_HAND]) {
        case RED:
            break;
        case ORANGE:
            move_cube(cube, Y_R180, 0, 0);
            break;
        case BLUE:
            move_cube(cube, Y_R90_C_CW, 0, 0);
            break;
        case GREEN:
            move_cube(cube, Y_R90_CW, 0, 0);
        }
        break;
    case RED:
    case ORANGE:
        move_cube(cube, (cube->hand_color[TOP_HAND] == RED ? Z_R90_CW : Z_R90_C_CW), 0, 0);

        switch (cube->hand_color[TOP_HAND]) {
        case YELLOW:
            break;
        case WHITE:
            move_cube(cube, X_R180, 0, 0);
            break;
        case BLUE:
            move_cube(cube, X_R90_C_CW, 0, 0);
            break;
        case GREEN:
            move_cube(cube, X_R90_CW, 0, 0);
        }
        break;
    case BLUE:
    case GREEN:
        move_cube(cube, (cube->hand_color[TOP_HAND] == BLUE ? X_R90_C_CW : X_R90_CW), 0, 0);

        switch (cube->hand_color[TOP_HAND]) {
        case YELLOW:
            break;
        case WHITE:
            move_cube(cube, Z_R180, 0, 0);
            break;
        case RED:
            move_cube(cube, Z_R90_CW, 0, 0);
            break;
        case ORANGE:
            move_cube(cube, Z_R90_C_CW, 0, 0);
        }
    }
}
