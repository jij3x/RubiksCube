#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rubiks_cube.h"
#include "cube_verifier.h"

int main(void) {
    cube_t *cube = create_cube(MAX_LAYERS);
    cube_t *testing_cube = clone_cube(cube);

    clock_t begin, end;
    int time_spent;
    begin = clock();

    if (same_cubes(cube, testing_cube))
        printf("same!\n");
    else
        printf("different!\n");

    end = clock();
    time_spent = (int) (end - begin);
    printf("exec time: %d ms", time_spent);

    destroy_cube(cube);

    return EXIT_SUCCESS;
}
