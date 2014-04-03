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
    cube_t *cube = create_cube(4096);

    bool result = is_valid_cube(cube);

    destroy_cube(cube);
    return result;
}

int main(int argc, char *argv[]) {
    test(&tc_identical_cubes, "is_valid_cube() happy case");
    test(&tc_is_valid_cube, "identical_cubes() happy case");

    print_stat();

    return EXIT_SUCCESS;
}
