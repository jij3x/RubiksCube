#include "rubiks_cube.h"

/*
 *     +---+
 *     | Y |
 * +---+---+---+---+
 * | O | B | R | G |
 * +---+---+---+---+
 *     | W |
 *     +---+
 */
cube_t *create_cube(unsigned int layers) {
	cube_t *cube = malloc(sizeof(cube_t));

	cube->layers = layers;
	color_t i;
	for (i = 0; i < TOTAL_COLOR; i++) {
		cube->faces[i] = malloc(layers * layers * sizeof(color_t));
		color_t *face_ptr = (color_t *) cube->faces[i];
		int j;
		for (j = 0; j < layers * layers; j++) {
			face_ptr[j] = i;
		}
	}
	cube->top = cube->faces[YELLOW];
	cube->bottom = cube->faces[WHITE];
	cube->front = cube->faces[BLUE];
	cube->back = cube->faces[GREEN];
	cube->right = cube->faces[RED];
	cube->left = cube->faces[ORANGE];

	return cube;
}

cube_t *create_3x3x3(void) {
	return create_cube(3);
}

void destroy_cube(cube_t *cube) {
	color_t i;
	for (i = 0; i < TOTAL_COLOR; i++) {
		free(cube->faces[i]);
	}

	free(cube);
}

cube_t *clone_cube(cube_t *cube) {
	cube_t* clone = malloc(sizeof(cube_t));

	unsigned int layers = cube->layers;
	clone->layers = layers;
	color_t i;
	for (i = 0; i < TOTAL_COLOR; i++) {
		clone->faces[i] = malloc(layers * layers * sizeof(color_t));
		color_t *face_ptr = (color_t *) clone->faces[i];
		int i;
		for (i = 0; i < layers * layers; i++) {
			face_ptr[i] = i;
		}

		if (cube->top == cube->faces[i])
			clone->back = clone->faces[i];
		else if (cube->bottom == cube->faces[i])
			clone->back = clone->faces[i];
		else if (cube->front == cube->faces[i])
			clone->front = clone->faces[i];
		else if (cube->back == cube->faces[i])
			clone->back = clone->faces[i];
		else if (cube->right == cube->faces[i])
			clone->right = clone->faces[i];
		else if (cube->left == cube->faces[i])
			clone->left = clone->faces[i];
	}

	return clone;
}

typedef enum {
	EAST, SOUTH, WEST, NORTH
} direction_t;

void row2row(color_t *src_ptr, unsigned int src_y, unsigned int src_x,
		direction_t src_dir, color_t *tgt_ptr, unsigned int tgt_y,
		unsigned int tgt_x, direction_t tgt_dir, unsigned int layers) {
	assert(layers > 0);
	assert(
			(src_dir == EAST && src_x == 0)
					|| (src_dir == WEST && src_x == layers - 1));
	assert(
			(tgt_dir == EAST && tgt_x == 0)
					|| (tgt_dir == WEST && tgt_x == layers - 1));

	int src_step = (src_dir == EAST ? 1 : -1);
	int tgt_step = (tgt_dir == EAST ? 1 : -1);
	int src_end = (src_dir == EAST ? layers - 1 : 0);
	color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
	color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
	int i, j;
	bool_t reached = false;
	for (i = src_x, j = tgt_x; !reached; i += src_step, j += tgt_step) {
		tgt_face[tgt_y][j] = src_face[src_y][i];
		reached = (i == src_end ? true : false);
	}
}

void row2col(color_t *src_ptr, unsigned int src_y, unsigned int src_x,
		direction_t src_dir, color_t *tgt_ptr, unsigned int tgt_y,
		unsigned int tgt_x, direction_t tgt_dir, unsigned int layers) {
	assert(layers > 0);
	assert(
			(src_dir == EAST && src_x == 0)
					|| (src_dir == WEST && src_x == layers - 1));
	assert(
			(tgt_dir == SOUTH && tgt_y == 0)
					|| (tgt_dir == NORTH && tgt_y == layers - 1));

	int src_step = (src_dir == EAST ? 1 : -1);
	int tgt_step = (tgt_dir == SOUTH ? 1 : -1);
	int src_end = (src_dir == EAST ? layers - 1 : 0);
	color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
	color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
	int i, j;
	bool_t reached = false;
	for (i = src_x, j = tgt_y; !reached; i += src_step, j += tgt_step) {
		tgt_face[j][tgt_x] = src_face[src_y][i];
		reached = (i == src_end ? true : false);
	}
}

void col2row(color_t *src_ptr, unsigned int src_y, unsigned int src_x,
		direction_t src_dir, color_t *tgt_ptr, unsigned int tgt_y,
		unsigned int tgt_x, direction_t tgt_dir, unsigned int layers) {
	assert(layers > 0);
	assert(
			(src_dir == SOUTH && src_y == 0)
					|| (src_dir == NORTH && src_y == layers - 1));
	assert(
			(tgt_dir == EAST && tgt_x == 0)
					|| (tgt_dir == WEST && tgt_x == layers - 1));

	int src_step = (src_dir == SOUTH ? 1 : -1);
	int tgt_step = (tgt_dir == EAST ? 1 : -1);
	int src_end = (src_dir == SOUTH ? layers - 1 : 0);
	color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
	color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
	int i, j;
	bool_t reached = false;
	for (i = src_y, j = tgt_x; !reached; i += src_step, j += tgt_step) {
		tgt_face[tgt_y][j] = src_face[i][src_x];
		reached = (i == src_end ? true : false);
	}
}

void col2col(color_t *src_ptr, unsigned int src_y, unsigned int src_x,
		direction_t src_dir, color_t *tgt_ptr, unsigned int tgt_y,
		unsigned int tgt_x, direction_t tgt_dir, unsigned int layers) {
	assert(layers > 0);
	assert(
			(src_dir == SOUTH && src_y == 0)
					|| (src_dir == NORTH && src_y == layers - 1));
	assert(
			(tgt_dir == SOUTH && tgt_y == 0)
					|| (tgt_dir == NORTH && tgt_y == layers - 1));

	int src_step = (src_dir == SOUTH ? 1 : -1);
	int tgt_step = (tgt_dir == SOUTH ? 1 : -1);
	int src_end = (src_dir == SOUTH ? layers - 1 : 0);
	color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
	color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;
	int i, j;
	bool_t reached = false;
	for (i = src_y, j = tgt_y; !reached; i += src_step, j += tgt_step) {
		tgt_face[j][tgt_x] = src_face[i][src_x];
		reached = (i == src_end ? true : false);
	}
}

void rotate_face_cw90(color_t *face_ptr, unsigned int layers) {
	color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

	unsigned int lvl, lvl_c, x, x_c, y, y_c;
	for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
		for (x = y = lvl, x_c = y_c = lvl_c; x < lvl_c;
				x++, y++, x_c--, y_c--) {
			color_t temp = face[lvl][x];
			face[lvl][x] = face[y_c][lvl];
			face[y_c][lvl] = face[lvl_c][x_c];
			face[lvl_c][x_c] = face[y][lvl_c];
			face[y][lvl_c] = temp;
		}
	}
}

void rotate_face_ccw90(color_t *face_ptr, unsigned int layers) {
	color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

	unsigned int lvl, lvl_c, x, x_c, y, y_c;
	for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
		for (x = y = lvl, x_c = y_c = lvl_c; x < lvl_c;
				x++, y++, x_c--, y_c--) {
			color_t temp = face[lvl][x];
			face[lvl][x] = face[y][lvl_c];
			face[y][lvl_c] = face[lvl_c][x_c];
			face[lvl_c][x_c] = face[y_c][lvl];
			face[y_c][lvl] = temp;
		}
	}
}

void rotate_face_180(color_t *face_ptr, unsigned int layers) {
	color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

	unsigned int lvl, lvl_c, x, x_c, y, y_c;
	for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
		for (x = y = lvl, x_c = y_c = lvl_c; x < lvl_c;
				x++, y++, x_c--, y_c--) {
			color_t temp = face[lvl][x];
			face[lvl][x] = face[lvl_c][x_c];
			face[lvl_c][x_c] = temp;

			temp = face[y][lvl_c];
			face[y][lvl_c] = face[y_c][lvl];
			face[y_c][lvl] = temp;
		}
	}
}

void move_right_cw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {
	int layers = cube->layers, i, j, last = layers - 1;
	if (start_l > end_l)
		SWAP(start_l, end_l);
	end_l = MIN(end_l, cube->layers);

	if (start_l == 0)
		rotate_face_cw90(cube->right, layers);
	if (end_l == layers - 1)
		rotate_face_ccw90(cube->left, layers);

	color_t *temp = malloc(sizeof(color_t) * layers);
	for (i = last - start_l, j = start_l; i >= last - end_l; i--, j++) {
		col2row(cube->front, 0, i, SOUTH, temp, 0, 0, EAST, layers);
		col2col(cube->bottom, 0, i, SOUTH, cube->front, 0, i, SOUTH, layers);
		col2col(cube->back, last, j, NORTH, cube->bottom, 0, i, SOUTH, layers);
		col2col(cube->top, 0, i, SOUTH, cube->back, last, j, NORTH, layers);
		row2col(temp, 0, 0, EAST, cube->top, 0, i, SOUTH, layers);
	}
	free(temp);
}

void move_right_ccw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_right_180(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_left_cw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_left_ccw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_left_180(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_front_cw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_front_ccw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_front_180(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_back_cw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_back_ccw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_back_180(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_top_cw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_top_ccw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_top_180(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_bottom_cw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_bottom_ccw90(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

void move_bottom_180(cube_t *cube, unsigned int start_l, unsigned int end_l) {

}

typedef void (*cube_move)(cube_t *, unsigned int, unsigned int);

static cube_move op_list[TOTAL_OPS] = { &move_right_cw90, &move_right_ccw90,
		&move_right_180, &move_left_cw90, &move_left_ccw90, &move_left_180,
		&move_top_cw90, &move_top_ccw90, &move_top_180, &move_bottom_cw90,
		&move_bottom_ccw90, &move_bottom_180, &move_front_cw90,
		&move_front_ccw90, &move_front_180, &move_back_cw90, &move_back_ccw90,
		&move_back_180 };

void move(cube_t *cube, step_t step, unsigned int start_l, unsigned int end_l) {
	if (start_l > end_l)
		return;

	op_list[step](cube, start_l, end_l);
}
