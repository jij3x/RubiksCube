#ifndef CUBE_INSPECTOR_H_
#define CUBE_INSPECTOR_H_

#include "globe_def.h"
#include "rubiks_cube.h"
#include "comm_util.h"

bool is_valid_cube(cube_t *cube);
bool is_valid_3x3x3(cube_t *cube);
bool identical_cubes(cube_t *restrict cube1, cube_t *restrict cube2);
bool is_solved_cube(cube_t *cube);

#endif /* CUBE_INSPECTOR_H_ */
