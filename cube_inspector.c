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

static inline bool valid_cornorblocks(cube_t *cube) {
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
