#include "cube_verifier.h"

bool_t is_valid_3x3x3(cube_t* cube) {
	color_t color;

	if (cube->layers != 3)
		return false;

	int count_tbl[TOTAL_COLOR];
	for (color = 0; color < TOTAL_COLOR; color++) {
		count_tbl[color] = 9;
	}

	for (color = 0; color < TOTAL_COLOR; color++) {
		color_t (*face)[3] = (color_t (*)[3]) cube->faces[color];
		if (face[1][1] != color)
			return false;

		int y, x;
		for (y = 0; y < 3; y++) {
			for (x = 0; x < 3; x++) {
				count_tbl[face[y][x]]--;
			}
		}
	}

	for (color = 0; color < TOTAL_COLOR; color++) {
		if (count_tbl[color] != 0)
			return false;
	}
	return true;
}
