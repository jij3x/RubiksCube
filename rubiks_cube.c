#include "rubiks_cube.h"

cube_t *create_cube(lyrnum_t layers) {
    layers = fmin(layers, MAX_LAYERS);
    layers = fmax(layers, MIN_LAYERS);
    lyrnum_t raw_face_size = pow(layers, 2) * sizeof(color_t) + ALIGNMENT - 1;

    color_t **raw_face_pool = malloc(sizeof(color_t *) * TOTAL_COLORS);
    color_t **face_pool = malloc(sizeof(color_t *) * TOTAL_COLORS);
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

    // At this point, cube has been created. One thing left to do is to set the sides.
    reset_sides(cube);

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
        clone->side_color[(side_t) i] = cube->side_color[(side_t) i];
        clone->sides[(side_t) i] = clone->faces[i];
    }

    return clone;
}

void reset_sides(cube_t *cube) {
    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        cube->face_offset[i] = NO_ROTATION;
        cube->sides[i] = cube->faces[i];
        cube->side_color[(side_t) i] = i;
    }
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
    color_t *temp_face = cube->sides[FRONT_SIDE];
    cube->sides[FRONT_SIDE] = cube->sides[BOTTOM_SIDE];
    cube->sides[BOTTOM_SIDE] = cube->sides[BACK_SIDE];
    cube->sides[BACK_SIDE] = cube->sides[TOP_SIDE];
    cube->sides[TOP_SIDE] = temp_face;

    color_t temp_idx = cube->side_color[FRONT_SIDE];
    cube->side_color[FRONT_SIDE] = cube->side_color[BOTTOM_SIDE];
    cube->side_color[BOTTOM_SIDE] = cube->side_color[BACK_SIDE];
    cube->side_color[BACK_SIDE] = cube->side_color[TOP_SIDE];
    cube->side_color[TOP_SIDE] = temp_idx;

    color_t rightside_c = cube->side_color[RIGHT_SIDE];
    cube->face_offset[rightside_c] = ra_tbl[cube->face_offset[rightside_c]][R_90_CW];
    color_t leftside_c = cube->side_color[LEFT_SIDE];
    cube->face_offset[leftside_c] = ra_tbl[cube->face_offset[leftside_c]][R_90_C_CW];
    color_t backside_c = cube->side_color[BACK_SIDE];
    cube->face_offset[backside_c] = ra_tbl[cube->face_offset[backside_c]][R_180];
    color_t bottomside_c = cube->side_color[BOTTOM_SIDE];
    cube->face_offset[bottomside_c] = ra_tbl[cube->face_offset[bottomside_c]][R_180];
}

void rotate_x_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->sides[FRONT_SIDE];
    cube->sides[FRONT_SIDE] = cube->sides[TOP_SIDE];
    cube->sides[TOP_SIDE] = cube->sides[BACK_SIDE];
    cube->sides[BACK_SIDE] = cube->sides[BOTTOM_SIDE];
    cube->sides[BOTTOM_SIDE] = temp_face;

    color_t temp_idx = cube->side_color[FRONT_SIDE];
    cube->side_color[FRONT_SIDE] = cube->side_color[TOP_SIDE];
    cube->side_color[TOP_SIDE] = cube->side_color[BACK_SIDE];
    cube->side_color[BACK_SIDE] = cube->side_color[BOTTOM_SIDE];
    cube->side_color[BOTTOM_SIDE] = temp_idx;

    color_t rightside_c = cube->side_color[RIGHT_SIDE];
    cube->face_offset[rightside_c] = ra_tbl[cube->face_offset[rightside_c]][R_90_C_CW];
    color_t leftside_c = cube->side_color[LEFT_SIDE];
    cube->face_offset[leftside_c] = ra_tbl[cube->face_offset[leftside_c]][R_90_CW];
    color_t backside_c = cube->side_color[BACK_SIDE];
    cube->face_offset[backside_c] = ra_tbl[cube->face_offset[backside_c]][R_180];
    color_t topside_c = cube->side_color[TOP_SIDE];
    cube->face_offset[topside_c] = ra_tbl[cube->face_offset[topside_c]][R_180];
}

void rotate_x_180(cube_t * cube) {
    color_t *temp_face = cube->sides[FRONT_SIDE];
    cube->sides[FRONT_SIDE] = cube->sides[BACK_SIDE];
    cube->sides[BACK_SIDE] = temp_face;
    color_t temp_idx = cube->side_color[FRONT_SIDE];
    cube->side_color[FRONT_SIDE] = cube->side_color[BACK_SIDE];
    cube->side_color[BACK_SIDE] = temp_idx;

    temp_face = cube->sides[TOP_SIDE];
    cube->sides[TOP_SIDE] = cube->sides[BOTTOM_SIDE];
    cube->sides[BOTTOM_SIDE] = temp_face;
    temp_idx = cube->side_color[TOP_SIDE];
    cube->side_color[TOP_SIDE] = cube->side_color[BOTTOM_SIDE];
    cube->side_color[BOTTOM_SIDE] = temp_idx;

    color_t rightside_c = cube->side_color[RIGHT_SIDE];
    cube->face_offset[rightside_c] = ra_tbl[cube->face_offset[rightside_c]][R_180];
    color_t leftside_c = cube->side_color[LEFT_SIDE];
    cube->face_offset[leftside_c] = ra_tbl[cube->face_offset[leftside_c]][R_180];
    color_t frontside_c = cube->side_color[FRONT_SIDE];
    cube->face_offset[frontside_c] = ra_tbl[cube->face_offset[frontside_c]][R_180];
    color_t backside_c = cube->side_color[BACK_SIDE];
    cube->face_offset[backside_c] = ra_tbl[cube->face_offset[backside_c]][R_180];
}

void rotate_y_90_cw(cube_t * cube) {
    color_t *temp_face = cube->sides[RIGHT_SIDE];
    cube->sides[RIGHT_SIDE] = cube->sides[FRONT_SIDE];
    cube->sides[FRONT_SIDE] = cube->sides[LEFT_SIDE];
    cube->sides[LEFT_SIDE] = cube->sides[BACK_SIDE];
    cube->sides[BACK_SIDE] = temp_face;

    color_t temp_idx = cube->side_color[RIGHT_SIDE];
    cube->side_color[RIGHT_SIDE] = cube->side_color[FRONT_SIDE];
    cube->side_color[FRONT_SIDE] = cube->side_color[LEFT_SIDE];
    cube->side_color[LEFT_SIDE] = cube->side_color[BACK_SIDE];
    cube->side_color[BACK_SIDE] = temp_idx;

    color_t topside_c = cube->side_color[TOP_SIDE];
    cube->face_offset[topside_c] = ra_tbl[cube->face_offset[topside_c]][R_90_C_CW];
    color_t bottomside_c = cube->side_color[BOTTOM_SIDE];
    cube->face_offset[bottomside_c] = ra_tbl[cube->face_offset[bottomside_c]][R_90_CW];
}

void rotate_y_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->sides[RIGHT_SIDE];
    cube->sides[RIGHT_SIDE] = cube->sides[BACK_SIDE];
    cube->sides[BACK_SIDE] = cube->sides[LEFT_SIDE];
    cube->sides[LEFT_SIDE] = cube->sides[FRONT_SIDE];
    cube->sides[FRONT_SIDE] = temp_face;

    color_t temp_idx = cube->side_color[RIGHT_SIDE];
    cube->side_color[RIGHT_SIDE] = cube->side_color[BACK_SIDE];
    cube->side_color[BACK_SIDE] = cube->side_color[LEFT_SIDE];
    cube->side_color[LEFT_SIDE] = cube->side_color[FRONT_SIDE];
    cube->side_color[FRONT_SIDE] = temp_idx;

    color_t topside_c = cube->side_color[TOP_SIDE];
    cube->face_offset[topside_c] = ra_tbl[cube->face_offset[topside_c]][R_90_CW];
    color_t bottomside_c = cube->side_color[BOTTOM_SIDE];
    cube->face_offset[bottomside_c] = ra_tbl[cube->face_offset[bottomside_c]][R_90_C_CW];
}

void rotate_y_180(cube_t * cube) {
    color_t *temp_face = cube->sides[RIGHT_SIDE];
    cube->sides[RIGHT_SIDE] = cube->sides[LEFT_SIDE];
    cube->sides[LEFT_SIDE] = temp_face;
    color_t temp_idx = cube->side_color[RIGHT_SIDE];
    cube->side_color[RIGHT_SIDE] = cube->side_color[LEFT_SIDE];
    cube->side_color[LEFT_SIDE] = temp_idx;

    temp_face = cube->sides[FRONT_SIDE];
    cube->sides[FRONT_SIDE] = cube->sides[BACK_SIDE];
    cube->sides[BACK_SIDE] = temp_face;
    temp_idx = cube->side_color[FRONT_SIDE];
    cube->side_color[FRONT_SIDE] = cube->side_color[BACK_SIDE];
    cube->side_color[BACK_SIDE] = temp_idx;

    color_t topside_c = cube->side_color[TOP_SIDE];
    cube->face_offset[topside_c] = ra_tbl[cube->face_offset[topside_c]][R_180];
    color_t bottomside_c = cube->side_color[BOTTOM_SIDE];
    cube->face_offset[bottomside_c] = ra_tbl[cube->face_offset[bottomside_c]][R_180];
}

void rotate_z_90_cw(cube_t * cube) {
    color_t *temp_face = cube->sides[RIGHT_SIDE];
    cube->sides[RIGHT_SIDE] = cube->sides[TOP_SIDE];
    cube->sides[TOP_SIDE] = cube->sides[LEFT_SIDE];
    cube->sides[LEFT_SIDE] = cube->sides[BOTTOM_SIDE];
    cube->sides[BOTTOM_SIDE] = temp_face;

    color_t temp_idx = cube->side_color[RIGHT_SIDE];
    cube->side_color[RIGHT_SIDE] = cube->side_color[TOP_SIDE];
    cube->side_color[TOP_SIDE] = cube->side_color[LEFT_SIDE];
    cube->side_color[LEFT_SIDE] = cube->side_color[BOTTOM_SIDE];
    cube->side_color[BOTTOM_SIDE] = temp_idx;

    color_t frontside_c = cube->side_color[FRONT_SIDE];
    cube->face_offset[frontside_c] = ra_tbl[cube->face_offset[frontside_c]][R_90_CW];
    color_t backside_c = cube->side_color[BACK_SIDE];
    cube->face_offset[backside_c] = ra_tbl[cube->face_offset[backside_c]][R_90_C_CW];
    color_t rightside_c = cube->side_color[RIGHT_SIDE];
    cube->face_offset[rightside_c] = ra_tbl[cube->face_offset[rightside_c]][R_90_CW];
    color_t leftside_c = cube->side_color[LEFT_SIDE];
    cube->face_offset[leftside_c] = ra_tbl[cube->face_offset[leftside_c]][R_90_CW];
    color_t topside_c = cube->side_color[TOP_SIDE];
    cube->face_offset[topside_c] = ra_tbl[cube->face_offset[topside_c]][R_90_CW];
    color_t bottomside_c = cube->side_color[BOTTOM_SIDE];
    cube->face_offset[bottomside_c] = ra_tbl[cube->face_offset[bottomside_c]][R_90_CW];
}

void rotate_z_90_c_cw(cube_t * cube) {
    color_t *temp_face = cube->sides[RIGHT_SIDE];
    cube->sides[RIGHT_SIDE] = cube->sides[BOTTOM_SIDE];
    cube->sides[BOTTOM_SIDE] = cube->sides[LEFT_SIDE];
    cube->sides[LEFT_SIDE] = cube->sides[TOP_SIDE];
    cube->sides[TOP_SIDE] = temp_face;

    color_t temp_idx = cube->side_color[RIGHT_SIDE];
    cube->side_color[RIGHT_SIDE] = cube->side_color[BOTTOM_SIDE];
    cube->side_color[BOTTOM_SIDE] = cube->side_color[LEFT_SIDE];
    cube->side_color[LEFT_SIDE] = cube->side_color[TOP_SIDE];
    cube->side_color[TOP_SIDE] = temp_idx;

    color_t frontside_c = cube->side_color[FRONT_SIDE];
    cube->face_offset[frontside_c] = ra_tbl[cube->face_offset[frontside_c]][R_90_C_CW];
    color_t backside_c = cube->side_color[BACK_SIDE];
    cube->face_offset[backside_c] = ra_tbl[cube->face_offset[backside_c]][R_90_CW];
    color_t rightside_c = cube->side_color[RIGHT_SIDE];
    cube->face_offset[rightside_c] = ra_tbl[cube->face_offset[rightside_c]][R_90_C_CW];
    color_t leftside_c = cube->side_color[LEFT_SIDE];
    cube->face_offset[leftside_c] = ra_tbl[cube->face_offset[leftside_c]][R_90_C_CW];
    color_t topside_c = cube->side_color[TOP_SIDE];
    cube->face_offset[topside_c] = ra_tbl[cube->face_offset[topside_c]][R_90_C_CW];
    color_t bottomside_c = cube->side_color[BOTTOM_SIDE];
    cube->face_offset[bottomside_c] = ra_tbl[cube->face_offset[bottomside_c]][R_90_C_CW];
}

void rotate_z_180(cube_t * cube) {
    color_t *temp_face = cube->sides[RIGHT_SIDE];
    cube->sides[RIGHT_SIDE] = cube->sides[LEFT_SIDE];
    cube->sides[LEFT_SIDE] = temp_face;
    color_t temp_idx = cube->side_color[RIGHT_SIDE];
    cube->side_color[RIGHT_SIDE] = cube->side_color[LEFT_SIDE];
    cube->side_color[LEFT_SIDE] = temp_idx;

    temp_face = cube->sides[TOP_SIDE];
    cube->sides[TOP_SIDE] = cube->sides[BOTTOM_SIDE];
    cube->sides[BOTTOM_SIDE] = temp_face;
    temp_idx = cube->side_color[TOP_SIDE];
    cube->side_color[TOP_SIDE] = cube->side_color[BOTTOM_SIDE];
    cube->side_color[BOTTOM_SIDE] = temp_idx;

    color_t frontside_c = cube->side_color[FRONT_SIDE];
    cube->face_offset[frontside_c] = ra_tbl[cube->face_offset[frontside_c]][R_180];
    color_t backside_c = cube->side_color[BACK_SIDE];
    cube->face_offset[backside_c] = ra_tbl[cube->face_offset[backside_c]][R_180];
    color_t rightside_c = cube->side_color[RIGHT_SIDE];
    cube->face_offset[rightside_c] = ra_tbl[cube->face_offset[rightside_c]][R_180];
    color_t leftside_c = cube->side_color[LEFT_SIDE];
    cube->face_offset[leftside_c] = ra_tbl[cube->face_offset[leftside_c]][R_180];
}

typedef void (*turn_func_t)(cube_t *, lyrnum_t, lyrnum_t);
typedef void (*rotate_func_t)(cube_t *); // layers does not matter to rotate functions
typedef union move_func_u {
    turn_func_t turn;
    rotate_func_t rotate;
} move_func_t;

static const move_func_t move_ops[TOTAL_MOVES] = {
// (turn + rotate) functions
        (move_func_t) &turn_y_90_c_cw, // YELLOW side: R_90_CW
        (move_func_t) &turn_y_90_cw,   // YELLOW side: R_90_C_CW
        (move_func_t) &turn_y_180,     // YELLOW side: R_180
        (move_func_t) &turn_y_90_cw,   // WHITE side: R_90_CW
        (move_func_t) &turn_y_90_c_cw, // WHITE side: R_90_C_CW
        (move_func_t) &turn_y_180,     // WHITE side: R_180
        (move_func_t) &turn_x_90_cw,   // RED side: R_90_CW
        (move_func_t) &turn_x_90_c_cw, // RED side: R_90_C_CW
        (move_func_t) &turn_x_180,     // RED side: R_180
        (move_func_t) &turn_x_90_c_cw, // ORANGE side: R_90_CW
        (move_func_t) &turn_x_90_cw,   // ORANGE side: R_90_C_CW
        (move_func_t) &turn_x_180,     // ORANGE side: R_180
        (move_func_t) &turn_z_90_cw,   // BLUE side: R_90_CW
        (move_func_t) &turn_z_90_c_cw, // BLUE side: R_90_C_CW
        (move_func_t) &turn_z_180,     // BLUE side: R_180
        (move_func_t) &turn_z_90_c_cw, // GREEN side: R_90_CW
        (move_func_t) &turn_z_90_cw,   // GREEN side: R_90_C_CW
        (move_func_t) &turn_z_180,     // GREEN side: R_180
        (move_func_t) &rotate_x_90_cw,   // cube rotate : X_R90_CW
        (move_func_t) &rotate_x_90_c_cw, // cube rotate : X_R90_C_CW
        (move_func_t) &rotate_x_180,     // cube rotate : X_R180
        (move_func_t) &rotate_y_90_cw,   // cube rotate : Y_R90_CW
        (move_func_t) &rotate_y_90_c_cw, // cube rotate : Y_R90_C_CW
        (move_func_t) &rotate_y_180,     // cube rotate : Y_R180
        (move_func_t) &rotate_z_90_cw,   // cube rotate : Z_R90_CW
        (move_func_t) &rotate_z_90_c_cw, // cube rotate : Z_R90_C_CW
        (move_func_t) &rotate_z_180      // cube rotate : Z_R180
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

    color_t side_color = cube->side_color[(color_t) move / 3];
    if (side_color == YELLOW || side_color == RED || side_color == BLUE) {
        start_l = cube->layers - 1 - start_l;
        end_l = cube->layers - 1 - end_l;
        SWAP(start_l, end_l);
    }

    move_ops[(move_t) side_color * 3 + move % 3].turn(cube, start_l, end_l);
}
