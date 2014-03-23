#include "rubiks_cube.h"

cube_t* create_3x3x3(void) {
	return create_cube(3);
}

/*
 *     +---+
 *     | Y |
 * +---+---+---+---+
 * | O | B | R | G |
 * +---+---+---+---+
 *     | W |
 *     +---+
 */
cube_t* create_cube(int layers) {
	cube_t* cube = malloc(sizeof(cube_t));

	cube->layers = layers;
	color_t color;
	for (color = 0; color < TOTAL_COLOR; color++) {
		cube->faces[color] = malloc(layers * layers * sizeof(color_t));
		color_t (*face)[layers] = (color_t (*)[layers]) cube->faces[color];
		int y, x;
		for (y = 0; y < layers; y++) {
			for (x = 0; x < layers; x++) {
				face[y][x] = color;
			}
		}
	}
	cube->front = BLUE;
	cube->back = GREEN;
	cube->top = YELLOW;
	cube->bottom = WHITE;
	cube->right = RED;
	cube->left = ORANGE;

	return cube;
}

void destory_cube(cube_t* cube) {
	int i;
	for (i = 0; i < TOTAL_COLOR; i++) {
		free(cube->faces[i]);
	}
	free(cube);
}
