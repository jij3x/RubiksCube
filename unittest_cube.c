#include "unittest.h"
#include "rubiks_cube.h"
#include "cube_inspector.h"

bool tc_identical_cubes(void) {
    cube_t *cube1 = create_cube(1024);
    cube_t *cube2 = clone_cube(cube1);

    bool result = identical_cubes(cube1, cube2);

    destroy_cube(cube1);
    destroy_cube(cube2);
    return result;
}

bool tc_is_valid_cube(void) {
    cube_t *cube = create_cube(1024);

    bool result = is_valid_cube(cube);

    destroy_cube(cube);
    return result;
}

int main(int argc, char *argv[]) {
    struct timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec * t.tv_sec);

    test(&tc_identical_cubes, "identical_cubes() happy case");
    test(&tc_is_valid_cube, "is_valid_cube() happy case");

    print_stat();

    return EXIT_SUCCESS;
}
