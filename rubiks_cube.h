#ifndef RUBIKS_CUBE_H_
#define RUBIKS_CUBE_H_

#include "globe_inc.h"

/*
 *     +---+
 *     | Y |
 * +---+---+---+---+
 * | O | B | R | G |
 * +---+---+---+---+
 *     | W |
 *     +---+
 */

typedef enum {
	BLUE = 0,
	GREEN = 1,
	RED = 2,
	ORANGE = 3,
	WHITE = 4,
	YELLOW = 5,
	TOTAL_COLOR = 6,
	NONE = 100
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
	BOTTOM_189
} step_t;

typedef struct {
	int layers;
	color_t* faces[TOTAL_COLOR];
	color_t front, back, top, bottom, right, left;
} cube_t;

cube_t* create_3x3x3(void);
cube_t* create_cube(int layers);
void destory_cube(cube_t* cube);

#endif /* RUBIKS_CUBE_H_ */
