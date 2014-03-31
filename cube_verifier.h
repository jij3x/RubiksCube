#ifndef CUBE_VERIFIER_H_
#define CUBE_VERIFIER_H_

#include "globe_def.h"
#include "rubiks_cube.h"
#include "comm_util.h"

bool is_valid_cube(cube_t *cube);
bool is_valid_3x3x3(cube_t *cube);
bool identical_cubes(cube_t *cube1, cube_t *cube2);
bool is_solved_cube(cube_t *cube);

#endif /* CUBE_VERIFIER_H_ */
