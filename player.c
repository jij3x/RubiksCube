#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rubiks_cube.h"
#include "cube_verifier.h"

int main(void) {
    cube_t *cube = create_cube(MAX_LAYERS);

    clock_t begin, end;
    int time_spent;
    begin = clock();

    turn_x_90_cw(cube, 0, cube->layers - 1);
    printf("turning completed!\n");

    end = clock();
    time_spent = (int) (end - begin);
    printf("exec time: %d ms", time_spent);

    destroy_cube(cube);

    return EXIT_SUCCESS;
}
