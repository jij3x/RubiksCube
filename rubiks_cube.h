#ifndef RUBIKS_CUBE_H_
#define RUBIKS_CUBE_H_

#include "globe_inc.h"

typedef enum {
	BLUE = 0,
	GREEN,
	RED,
	ORANGE,
	WHITE,
	YELLOW,
	TOTAL_COLOR,
	DARK = 100,
	BRIGHT = 101
} color_t;

typedef enum {
	RIGHT_CW90 = 0,
	RIGHT_CCW90,
	RIGHT_180,
	LEFT_CW90,
	LEFT_CCW90,
	LEFT_180,
	FRONT_CW90,
	FRONT_CCW90,
	FRONT_180,
	BACK_CW90,
	BACK_CCW90,
	BACK_180,
	TOP_CW90,
	TOP_CCW90,
	TOP_180,
	BOTTOM_CW90,
	BOTTOM_CCW90,
	BOTTOM_180,
	TOTAL_OPS
} step_t;

typedef struct {
	unsigned int layers;
	color_t *faces[TOTAL_COLOR];
	color_t *top, *bottom, *front, *back, *right, *left;
} cube_t;

cube_t *create_3x3x3(void);
cube_t *create_cube(unsigned int layers);
void destroy_cube(cube_t *cube);
cube_t *clone_cube(cube_t *cube);

void move(cube_t *cube, step_t step, unsigned int start_l, unsigned int end_l);
void reset_coordinate(cube_t *cube);

#endif /* RUBIKS_CUBE_H_ */
