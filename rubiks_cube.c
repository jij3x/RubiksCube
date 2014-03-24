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
cube_t *create_cube(int layers) {
	cube_t *cube = malloc(sizeof(cube_t));

	cube->layers = layers;
	color_t it;
	for (it = 0; it < TOTAL_COLOR; it++) {
		cube->faces[it] = malloc(layers * layers * sizeof(color_t));
		color_t *face_ptr = (color_t *) cube->faces[it];
		int i;
		for (i = 0; i < layers * layers; i++) {
			face_ptr[i] = it;
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
	color_t it;
	for (it = 0; it < TOTAL_COLOR; it++) {
		free(cube->faces[it]);
	}

	free(cube);
}

cube_t *clone_cube(cube_t *cube) {
	cube_t* clone = malloc(sizeof(cube_t));

	int layers = cube->layers;
	clone->layers = layers;
	color_t it;
	for (it = 0; it < TOTAL_COLOR; it++) {
		clone->faces[it] = malloc(layers * layers * sizeof(color_t));
		color_t *face_ptr = (color_t *) clone->faces[it];
		int i;
		for (i = 0; i < layers * layers; i++) {
			face_ptr[i] = it;
		}

		if (cube->top == cube->faces[it])
			clone->back = clone->faces[it];
		else if (cube->bottom == cube->faces[it])
			clone->back = clone->faces[it];
		else if (cube->front == cube->faces[it])
			clone->front = clone->faces[it];
		else if (cube->back == cube->faces[it])
			clone->back = clone->faces[it];
		else if (cube->right == cube->faces[it])
			clone->right = clone->faces[it];
		else if (cube->left == cube->faces[it])
			clone->left = clone->faces[it];
	}

	return clone;
}

void copy_row(color_t *src_ptr, unsigned int src_start_y,
		unsigned int src_start_x, unsigned int src_end_y,
		unsigned int src_end_x, color_t *tgt_ptr, unsigned int tgt_start_y,
		unsigned int tgt_start_x, unsigned int tgt_end_y,
		unsigned int tgt_end_x) {
	int i, j, src_step, tgt_step;
	bool_t reached;

	int delta_x = src_start_x - src_end_x;
	int delta_y = src_start_y - src_end_y;
	int layers = MAX(ABS(delta_x), ABS(delta_y)) + 1;
	color_t (*src_face)[layers] = (color_t (*)[layers]) src_ptr;
	color_t (*tgt_face)[layers] = (color_t (*)[layers]) tgt_ptr;

	if (src_start_x == src_end_x) {
		if (tgt_start_x == tgt_end_x) {
			src_step = (src_start_y < src_end_y ? 1 : -1);
			tgt_step = (tgt_start_y < tgt_end_y ? 1 : -1);
			i = src_start_y, j = tgt_start_y;
			for (reached = false; !reached; i += src_step, j += tgt_step) {
				tgt_face[j][tgt_start_x] = src_face[i][src_start_x];
				reached = (i == src_end_y ? true : false);
			}
		} else { // (src_start_x == src_end_x) && (tgt_start_y == tgt_end_y)
			src_step = (src_start_y < src_end_y ? 1 : -1);
			tgt_step = (tgt_start_x < tgt_end_x ? 1 : -1);
			i = src_start_y, j = tgt_start_x;
			for (reached = false; !reached; i += src_step, j += tgt_step) {
				tgt_face[tgt_start_y][j] = src_face[i][src_start_x];
				reached = (i == src_end_y ? true : false);
			}
		}
	} else {
		if (tgt_start_x == tgt_end_x) { // (src_start_y == src_end_y)
			src_step = (src_start_x < src_end_x ? 1 : -1);
			tgt_step = (tgt_start_y < tgt_end_y ? 1 : -1);
			i = src_start_x, j = tgt_start_y;
			for (reached = false; !reached; i += src_step, j += tgt_step) {
				tgt_face[j][tgt_start_x] = src_face[src_start_y][i];
				reached = (i == src_end_x ? true : false);
			}
		} else { // (src_start_y == src_end_y) && (tgt_start_y == tgt_end_y)
			src_step = (src_start_x < src_end_x ? 1 : -1);
			tgt_step = (tgt_start_x < tgt_end_x ? 1 : -1);
			i = src_start_x, j = tgt_start_x;
			for (reached = false; !reached; i += src_step, j += tgt_step) {
				tgt_face[tgt_start_y][j] = src_face[src_start_y][i];
				reached = (i == src_end_x ? true : false);
			}
		}
	}
}

void rotate_face_cw90(color_t *face_ptr, int layers) {
	color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

	int lvl, lvl_c, x, x_c, y, y_c;
	for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
		x = y = lvl, x_c = y_c = lvl_c;
		for (; x < lvl_c; x++, y++, x_c--, y_c--) {
			color_t temp = face[lvl][x];
			face[lvl][x] = face[y_c][lvl];
			face[y_c][lvl] = face[lvl_c][x_c];
			face[lvl_c][x_c] = face[y][lvl_c];
			face[y][lvl_c] = temp;
		}
	}
}

void rotate_face_ccw90(color_t *face_ptr, int layers) {
	color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

	int lvl, lvl_c, x, x_c, y, y_c;
	for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
		x = y = lvl, x_c = y_c = lvl_c;
		for (; x < lvl_c; x++, y++, x_c--, y_c--) {
			color_t temp = face[lvl][x];
			face[lvl][x] = face[y][lvl_c];
			face[y][lvl_c] = face[lvl_c][x_c];
			face[lvl_c][x_c] = face[y_c][lvl];
			face[y_c][lvl] = temp;
		}
	}
}

void rotate_face_180(color_t *face_ptr, int layers) {
	color_t (*face)[layers] = (color_t (*)[layers]) face_ptr;

	int lvl, lvl_c, x, x_c, y, y_c;
	for (lvl = 0, lvl_c = layers - 1 - lvl; lvl < layers / 2; lvl++, lvl_c--) {
		x = y = lvl, x_c = y_c = lvl_c;
		for (; x < lvl_c; x++, y++, x_c--, y_c--) {
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
		copy_row(cube->front, 0, i, last, i, temp, 0, 0, 0, last);
		copy_row(cube->bottom, 0, i, last, i, cube->front, 0, i, last, i);
		copy_row(cube->back, last, j, 0, j, cube->bottom, 0, i, last, i);
		copy_row(cube->top, 0, i, last, i, cube->back, last, j, 0, j);
		copy_row(temp, 0, 0, 0, last, cube->top, 0, i, last, i);
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
