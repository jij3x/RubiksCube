#include "cube_inspector.h"
extern void rotate_coord(cube_t *cube, side_t side, lyrnum_t *restrict y, lyrnum_t *restrict x);
extern inline color_t read_face(cube_t *cube, side_t side, lyrnum_t y, lyrnum_t x);

static inline bool valid_sides(cube_t *cube) {
    // validate whether cube's internal sides registers
    if (opp_color[cube->side_color[YELLOW]] != cube->side_color[WHITE]
            || opp_color[cube->side_color[RED]] != cube->side_color[ORANGE]
            || opp_color[cube->side_color[BLUE]] != cube->side_color[GREEN])
        return false;

    for (side_t i = 0; i < TOTAL_SIDES; i++) {
        if (cube->sides[i] != cube->faces[cube->side_color[i]])
            return false;
    }

    return true;
}

static inline bool valid_facerotations(cube_t *cube) {
    cube_t *ref_cube = create_cube(MIN_LAYERS);

    typedef uint8_t aln_t;
    enum aln_e {
        topside_aligned, rightside_aligned, frontside_aligned
    };

    aln_t side_aligned = topside_aligned;
    switch (cube->side_color[TOP_SIDE]) {
    case WHITE:
        move_cube(ref_cube, X_R180, 0, 0);
        break;
    case RED:
        move_cube(ref_cube, Z_R90_CW, 0, 0);
        side_aligned = rightside_aligned;
        break;
    case ORANGE:
        move_cube(ref_cube, Z_R90_C_CW, 0, 0);
        side_aligned = rightside_aligned;
        break;
    case BLUE:
        move_cube(ref_cube, Y_R90_C_CW, 0, 0);
        side_aligned = frontside_aligned;
        break;
    case GREEN:
        move_cube(ref_cube, Y_R90_CW, 0, 0);
        side_aligned = frontside_aligned;
    }

    switch (side_aligned) {
    case topside_aligned:
        switch (cube->side_color[RIGHT_SIDE]) {
        case ORANGE:
            move_cube(ref_cube, Y_R180, 0, 0);
            break;
        case BLUE:
            move_cube(ref_cube, Y_R90_C_CW, 0, 0);
            break;
        case GREEN:
            move_cube(ref_cube, Y_R90_CW, 0, 0);
        }
        break;
    case rightside_aligned:
        switch (cube->side_color[TOP_SIDE]) {
        case WHITE:
            move_cube(ref_cube, X_R180, 0, 0);
            break;
        case BLUE:
            move_cube(ref_cube, X_R90_C_CW, 0, 0);
            break;
        case GREEN:
            move_cube(ref_cube, X_R90_CW, 0, 0);
        }
        break;
    case frontside_aligned:
        switch (cube->side_color[TOP_SIDE]) {
        case WHITE:
            move_cube(ref_cube, Z_R180, 0, 0);
            break;
        case RED:
            move_cube(ref_cube, X_R90_CW, 0, 0);
            break;
        case ORANGE:
            move_cube(ref_cube, X_R90_C_CW, 0, 0);
        }
    }

    destroy_cube(ref_cube);

    for (color_t i = 0; i < TOTAL_COLORS; i++) {
        if (ref_cube->face_offset[i] != cube->face_offset[i])
            return false;
    }

    return true;
}

static inline bool valid_total_colors(cube_t *cube) {
    lyrnum_t layers = cube->layers;
    for (lyrnum_t i = 0; i < (layers - 1) / 2 + 1; i++) {
        lyrnum_t total = (layers - i * 2) * 4 - 4;
        total = (total == 0 ? 1 : total);
        lyrnum_t count[TOTAL_COLORS];
        for (color_t j = 0; j < TOTAL_COLORS; j++) {
            count[j] = total;
        }

        if (total == 1) {
            lyrnum_t center = pow(layers, 2);
            for (color_t j = 0; j < TOTAL_COLORS; j++) {
                count[cube->faces[j][center]]--;
            }
        } else {
            for (color_t j = 0; j < TOTAL_COLORS; j++) {
                color_t (*face)[layers] = (color_t (*)[layers]) cube->faces[j];
                for (lyrnum_t k = i, k_c = layers - 1 - i; k < layers - 1 - i; k++, k_c--) {
                    count[face[i][k]]--;
                    count[face[k][layers - 1 - i]]--;
                    count[face[layers - 1 - i][k_c]]--;
                    count[face[k_c][i]]--;
                }
            }
        }

        for (color_t j = 0; j < TOTAL_COLORS; j++) {
            if (count[j] != 0)
                return false;
        }
    }

    return true;
}

static inline bool valid_centralpieces(cube_t *cube) {
    // central pieces can drift in even layers cube
    if (cube->layers % 2 == 0)
        return true;

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

typedef struct cblk {
    color_t c1, c2, c3;
} cblk_t;

uint32_t cblk_hash(void *key) {
    cblk_t *cblk = (cblk_t *) key;
    if (cblk->c1 > cblk->c2)
        SWAP(cblk->c1, cblk->c2);
    if (cblk->c2 > cblk->c3)
        SWAP(cblk->c2, cblk->c3);
    if (cblk->c1 > cblk->c2)
        SWAP(cblk->c1, cblk->c2);

    return cblk->c1 * pow(TOTAL_COLORS, 2) + cblk->c2 * TOTAL_COLORS + cblk->c3;
}

int cblk_cmp(void *cblk1, void *cblk2) {
    return cblk_hash(cblk1) - cblk_hash(cblk2);
}

static inline bool valid_cornorblocks(cube_t *cube) {
    cblk_t ref_cblks[8] = {
    //
            { .c1 = YELLOW, .c2 = BLUE, .c3 = RED }, //
            { .c1 = YELLOW, .c2 = BLUE, .c3 = ORANGE }, //
            { .c1 = YELLOW, .c2 = GREEN, .c3 = RED }, //
            { .c1 = YELLOW, .c2 = GREEN, .c3 = ORANGE }, //
            { .c1 = WHITE, .c2 = BLUE, .c3 = RED }, //
            { .c1 = WHITE, .c2 = BLUE, .c3 = ORANGE }, //
            { .c1 = WHITE, .c2 = GREEN, .c3 = RED }, //
            { .c1 = WHITE, .c2 = GREEN, .c3 = ORANGE } //
    };

    uint32_t init_cnt = 1;

    kv_pair_t kv_set[8] = {
    //
            { .key = &ref_cblks[0], .value = &init_cnt, .vlen = sizeof(init_cnt) }, //
            { .key = &ref_cblks[1], .value = &init_cnt, .vlen = sizeof(init_cnt) }, //
            { .key = &ref_cblks[2], .value = &init_cnt, .vlen = sizeof(init_cnt) }, //
            { .key = &ref_cblks[3], .value = &init_cnt, .vlen = sizeof(init_cnt) }, //
            { .key = &ref_cblks[4], .value = &init_cnt, .vlen = sizeof(init_cnt) }, //
            { .key = &ref_cblks[5], .value = &init_cnt, .vlen = sizeof(init_cnt) }, //
            { .key = &ref_cblks[6], .value = &init_cnt, .vlen = sizeof(init_cnt) }, //
            { .key = &ref_cblks[7], .value = &init_cnt, .vlen = sizeof(init_cnt) } //
    };

    int rc;
    perf_htbl_t *tbl = perf_htbl_create(kv_set, 8, &cblk_hash, &cblk_cmp, NULL, &rc);

    lyrnum_t layers = cube->layers;

    color_t (*y_face)[layers] = (color_t (*)[layers]) cube->faces[YELLOW];
    color_t (*w_face)[layers] = (color_t (*)[layers]) cube->faces[WHITE];
    color_t (*r_face)[layers] = (color_t (*)[layers]) cube->faces[RED];
    color_t (*o_face)[layers] = (color_t (*)[layers]) cube->faces[ORANGE];
    color_t (*b_face)[layers] = (color_t (*)[layers]) cube->faces[BLUE];
    color_t (*g_face)[layers] = (color_t (*)[layers]) cube->faces[GREEN];

    lyrnum_t last = layers - 1;
    cblk_t testing_cblks[8] = {
    //
            { .c1 = y_face[0][0], .c2 = o_face[0][0], .c3 = g_face[0][last] }, //
            { .c1 = y_face[0][last], .c2 = r_face[0][last], .c3 = g_face[0][0] }, //
            { .c1 = y_face[last][last], .c2 = r_face[0][0], .c3 = b_face[0][last] }, //
            { .c1 = y_face[last][0], .c2 = b_face[0][0], .c3 = o_face[0][last] }, //
            { .c1 = w_face[0][0], .c2 = b_face[last][0], .c3 = o_face[last][last] }, //
            { .c1 = w_face[0][last], .c2 = b_face[last][last], .c3 = r_face[last][0] }, //
            { .c1 = w_face[last][last], .c2 = r_face[last][last], .c3 = g_face[0][last] }, //
            { .c1 = w_face[last][0], .c2 = o_face[0][last], .c3 = g_face[last][last] } //
    };

    for (size_t i = 0; i < 8; i++) {
        uint32_t *count = perf_htbl_get(tbl, &testing_cblks[i]);
        if (count == NULL || *count != 1) {
            perf_htbl_destroy(tbl);
            return false;
        }
        (*count)--;
    }

    perf_htbl_destroy(tbl);
    return true;
}

bool is_valid_cube(cube_t *cube) {
    if (!valid_sides(cube))
        return false;
    if (!valid_facerotations(cube))
        return false;
    if (!valid_total_colors(cube))
        return false;
    if (!valid_centralpieces(cube))
        return false;
    if (!valid_edgeblocks(cube))
        return false;
    if (!valid_cornorblocks(cube))
        return false;

    return true;
}

bool identical_cubes(cube_t *restrict cube1, cube_t *restrict cube2) {
    if (cube1->layers != cube2->layers)
        return false;

    const move_t check_list[24] = {
    // initial position is "YELLOW at TOP, BLUE in front"
            Y_R90_CW, Y_R90_CW, Y_R90_CW, Y_R90_CW,   // stop at "YELLOW at top, BLUE in front"
            X_R90_CW, Y_R90_CW, Y_R90_CW, Y_R90_CW,   // stop at "BLUE at TOP, RED in front"
            Z_R90_CW, Y_R90_CW, Y_R90_CW, Y_R90_CW,   // stop at "WHITE at TOP, BLUE in front"
            X_R90_C_CW, Y_R90_CW, Y_R90_CW, Y_R90_CW, // stop at "GREEN at TOP, ORANGE in front"
            X_R90_CW, Y_R90_CW, Y_R90_CW, Y_R90_CW,   // stop at "ORANGE at TOP, YELLOW in front"
            X_R180, Y_R90_CW, Y_R90_CW, Y_R90_CW      // stop at "RED at TOP, GREEN in front"
            };

    bool matched = false;
    for (size_t i = 0; i < 24; i++) {
        move_cube(cube2, check_list[i], 0, 0);
        if (matched)
            continue;

        for (side_t j = 0; j < TOTAL_SIDES; j++) {
            for (lyrnum_t y = 0; y < cube1->layers; y++) {
                for (lyrnum_t x = 0; x < cube1->layers; x++) {
                    if (read_face(cube1, j, y, x) != read_face(cube2, j, y, x))
                        goto mismatched;
                }
            }
        }
        matched = true;

        mismatched: continue;
    }

    // restore the original side position
    move_cube(cube2, Z_R90_C_CW, 0, 0);
    move_cube(cube2, Y_R180, 0, 0);

    return true;
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

    // colors on opposite sides must be opposite
    if (cube->faces[YELLOW][0] != opp_color[cube->faces[WHITE][0]]
            || cube->faces[RED][0] != opp_color[cube->faces[ORANGE][0]]
            || cube->faces[BLUE][0] != opp_color[cube->faces[GREEN][0]])
        return false;

    return true;
}

bool is_recoverable_cube(cube_t *cube) {
    return true;
}

void recover_cube(cube_t *cube) {

}
