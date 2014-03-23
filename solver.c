#include <stdio.h>
#include <stdlib.h>

#include "rubiks_cube.h"
#include "cube_verifier.h"

int main(void) {
	cube_t* cube = create_3x3x3();

	if (is_valid_3x3x3(cube) == true)
		printf("valid 3x3x3!!\n");
	else
		printf("invalid 3x3x3!!\n");

	destory_cube(cube);

	return EXIT_SUCCESS;
}

