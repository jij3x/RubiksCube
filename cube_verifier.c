#include "cube_verifier.h"

bool valid_total_color(cube_t *cube) {
    return true;
}

bool valid_hands(cube_t *cube) {
    if (opp_color[cube->hand_idx[YELLOW]] != cube->hand_idx[WHITE]
            || opp_color[cube->hand_idx[RED]] != cube->hand_idx[ORANGE]
            || opp_color[cube->hand_idx[BLUE]] != cube->hand_idx[GREEN])
        return false;

    for (hand_t i = 0; i < TOTAL_HANDS; i++) {
        if (cube->hands[i] != cube->faces[cube->hand_idx[i]])
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

    cube_t *testing_cube = clone_cube(cube2);
    lyrnum_t layers = testing_cube->layers;
    sll_t *verify_steps = sll_create(sizeof(sll_t), (free_func_t) &sll_destroy);

    // step 1
    sll_t *maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move01 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move01);
    sll_append(verify_steps, maneuver);
/*
    // step 2
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move02 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move02);
    sll_append(verify_steps, maneuver);

    // step 3
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move03 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move03);
    sll_append(verify_steps, maneuver);

    // step 4
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move04 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move04);
    sll_append(verify_steps, maneuver);

    // step 5
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move05_01 = { .mof = RIGHT_R90_C_CW, .start_l = 0, .end_l = layers - 1 };
    mvrstp_t move05_02 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move05_01);
    sll_append(maneuver, &move05_02);
    sll_append(verify_steps, maneuver);

    // step 6
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move06 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move06);
    sll_append(verify_steps, maneuver);

    // step 7
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move07 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move07);
    sll_append(verify_steps, maneuver);

    // step 8
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move08 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move08);
    sll_append(verify_steps, maneuver);

    // step 9
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move09_01 = { .mof = RIGHT_R90_C_CW, .start_l = 0, .end_l = layers - 1 };
    mvrstp_t move09_02 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move09_01);
    sll_append(maneuver, &move09_02);
    sll_append(verify_steps, maneuver);

    // step 10
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move10 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move10);
    sll_append(verify_steps, maneuver);

    // step 11
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move11 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move11);
    sll_append(verify_steps, maneuver);

    // step 12
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move12 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move12);
    sll_append(verify_steps, maneuver);

    // step 13
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move13_01 = { .mof = RIGHT_R90_C_CW, .start_l = 0, .end_l = layers - 1 };
    mvrstp_t move13_02 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move13_01);
    sll_append(maneuver, &move13_02);
    sll_append(verify_steps, maneuver);

    // step 14
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move14 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move14);
    sll_append(verify_steps, maneuver);

    // step 15
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move15 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move15);
    sll_append(verify_steps, maneuver);

    // step 16
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move16 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move16);
    sll_append(verify_steps, maneuver);

    // step 17
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move17_01 = { .mof = RIGHT_R90_C_CW, .start_l = 0, .end_l = layers - 1 };
    mvrstp_t move17_02 = { .mof = BOTTOM_R90_CW, .start_l = 0, .end_l = layers - 1 };
    mvrstp_t move17_03 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move17_01);
    sll_append(maneuver, &move17_02);
    sll_append(maneuver, &move17_03);
    sll_append(verify_steps, maneuver);

    // step 18
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move18 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move18);
    sll_append(verify_steps, maneuver);

    // step 19
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move19 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move19);
    sll_append(verify_steps, maneuver);

    // step 20
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move20 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move20);
    sll_append(verify_steps, maneuver);

    // step 21
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move21_01 = { .mof = BOTTOM_R180, .start_l = 0, .end_l = layers - 1 };
    mvrstp_t move21_02 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move21_01);
    sll_append(maneuver, &move21_02);
    sll_append(verify_steps, maneuver);

    // step 22
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move22 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move22);
    sll_append(verify_steps, maneuver);

    // step 23
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move23 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move23);
    sll_append(verify_steps, maneuver);

    // step 24
    maneuver = sll_create(sizeof(mvrstp_t), NULL);
    mvrstp_t move24 = { .mof = FRONT_R90_CW, .start_l = 0, .end_l = layers - 1 };
    sll_append(maneuver, &move24);
    sll_append(verify_steps, maneuver);
*/
    sll_node_t *steps;
    while ((steps = sll_get_next(verify_steps))) {
        maneuver_cube(testing_cube, (sll_t *)steps->elem_ptr);
        bool diff = false;
        for (color_t i = 0; i < TOTAL_COLORS && !diff; i++) {
            for (lyrnum_t j; j < pow(cube1->layers, 2); j++) {
                if (cube1->faces[i][j] != testing_cube->faces[i][j]) {
                    diff = true;
                    break;
                }
            }
        }

        if (!diff) {
            sll_destroy(verify_steps);
            destroy_cube(testing_cube);
            return true;
        }
    }

    sll_destroy(verify_steps);
    destroy_cube(testing_cube);
    return false;
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
