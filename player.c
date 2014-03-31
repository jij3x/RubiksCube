#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rubiks_cube.h"
#include "cube_verifier.h"

int main(void) {
    cube_t *cube = create_cube(4);
    cube_t *testing_cube = clone_cube(cube);

    rotate_x_90_cw(testing_cube, 0, testing_cube->layers - 1);
    rotate_y_90_cw(testing_cube, 0, testing_cube->layers - 1);
    rotate_z_90_cw(testing_cube, 0, testing_cube->layers - 1);

    clock_t begin, end;
    int time_spent;
    begin = clock();

    if (identical_cubes(cube, testing_cube))
        printf("identical!!\n");
    else
        printf("different cubes!!\n");

    //move_cube(cube, RIGHT_HAND, R_90_CW, 0, 0);
    //rotate_x_90_cw(cube, 2,2);

    end = clock();
    time_spent = (int) (end - begin);
    printf("exec time: %d ms", time_spent);

    destroy_cube(cube);

    return EXIT_SUCCESS;
}
