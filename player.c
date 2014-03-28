#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rubiks_cube.h"
#include "cube_verifier.h"

int main(void) {
    //cube_t *cube = create_3x3x3();
    cube_t *cube = create_cube(4096);

    if (is_valid_3x3x3(cube) == true)
        printf("valid 3x3x3!!\n");
    else
        printf("invalid 3x3x3!!\n");

    clock_t begin, end;
    int time_spent;
    begin = clock();

    move(cube, RIGHT_CW90, 0, 4096);

    end = clock();
    time_spent = (int) (end - begin);
    printf("exec time: %d ms", time_spent);

    destroy_cube(cube);

    return EXIT_SUCCESS;
}
