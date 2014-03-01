package rubikscube;

import java.util.ArrayList;
import java.util.HashMap;

public class RubiksCube {
	/*
	 *     +---+
	 *     | Y |
	 * +---+---+---+---+
	 * | B | R | G | O |
	 * +---+---+---+---+
	 *     | W |
	 *     +---+
	 */
	private HashMap<Color, Color[][]> cube;
	private Color[][] top, bottom, left, right, front, back;

	public RubiksCube(HashMap<Color, Color[][]> cube) {
		this.cube = new HashMap<Color, Color[][]>(cube.size());
		for (Color[][] face : cube.values()) {
			Color[][] cloneFace = new Color[face.length][face[0].length];
			for (int y = 0; y < cloneFace.length; y++) {
				for (int x = 0; x < cloneFace[0].length; x++) {
					cloneFace[y][x] = face[y][x];
				}
			}
			this.cube.put(cloneFace[1][1], cloneFace);
		}

		this.top = cube.get(Color.Yellow);
		this.bottom = cube.get(Color.White);
		this.left = cube.get(Color.Blue);
		this.right = cube.get(Color.Green);
		this.front = cube.get(Color.Red);
		this.back = cube.get(Color.Orange);
	}

	private void rotateCW90(Color[][] face) {
		for (int i = 0; i < 2; i++) {
			Color temp = face[0][i];
			face[0][i] = face[2 - i][0];
			face[2 - i][0] = face[2][2 - i];
			face[2][2 - i] = face[2 - i][2];
			face[2 - i][2] = temp;
		}
	}

	private void rotateCCW90(Color[][] face) {
		for (int i = 0; i < 2; i++) {
			Color temp = face[0][i];
			face[0][i] = face[i][2];
			face[i][2] = face[2][2 - i];
			face[2][2 - i] = face[2 - i][0];
			face[2 - i][0] = temp;
		}
	}

	private void rotate180(Color[][] face) {
		for (int i = 0; i < 2; i++) {
			Color temp = face[0][i];
			face[0][i] = face[2][2 - i];
			face[2][2 - i] = temp;

			temp = face[i][2];
			face[i][2] = face[2 - i][0];
			face[2 - i][0] = temp;
		}
	}

	public void turnCube(ArrayList<Move> steps) {
		Color tempColor;
		Color[][] tempFace;

		for (int idx = 0; idx < steps.size(); idx++) {
			switch (steps.get(idx)) {
			case BackCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.right[j][2];
					this.right[j][2] = this.bottom[2][2 - j];
					this.bottom[2][2 - j] = this.left[2 - j][0];
					this.left[2 - j][0] = this.top[0][j];
					this.top[0][j] = tempColor;
				}
				rotateCW90(this.back);
				break;
			case BackCCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.right[j][2];
					this.right[j][2] = this.top[0][j];
					this.top[0][j] = this.left[2 - j][0];
					this.left[2 - j][0] = this.bottom[2][2 - j];
					this.bottom[2][2 - j] = tempColor;
				}
				rotateCCW90(this.back);
				break;
			case Back180:
				for (int j = 0; j < 3; j++) {
					tempColor = this.right[j][2];
					this.right[j][2] = this.left[2 - j][0];
					this.left[2 - j][0] = tempColor;

					tempColor = this.top[0][j];
					this.top[0][j] = this.bottom[2][2 - j];
					this.bottom[2][2 - j] = tempColor;
				}
				rotate180(this.back);
				break;
			case BottomCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[2][j];
					this.front[2][j] = this.left[2][j];
					this.left[2][j] = this.back[2][j];
					this.back[2][j] = this.right[2][j];
					this.right[2][j] = tempColor;
				}
				rotateCW90(this.bottom);
				break;
			case BottomCCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[2][j];
					this.front[2][j] = this.right[2][j];
					this.right[2][j] = this.back[2][j];
					this.back[2][j] = this.left[2][j];
					this.left[2][j] = tempColor;
				}
				rotateCCW90(this.bottom);
				break;
			case Bottom180:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[2][j];
					this.front[2][j] = this.back[2][j];
					this.back[2][j] = tempColor;

					tempColor = this.left[2][j];
					this.left[2][j] = this.right[2][j];
					this.right[2][j] = tempColor;
				}
				rotate180(this.bottom);
				break;
			case FrontCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.top[2][j];
					this.top[2][j] = this.left[2 - j][2];
					this.left[2 - j][2] = this.bottom[0][j];
					this.bottom[0][j] = this.right[2 - j][0];
					this.right[2 - j][0] = tempColor;
				}
				rotateCW90(this.front);
				break;
			case FrontCCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.top[2][j];
					this.top[2][j] = this.right[j][0];
					this.right[j][0] = this.bottom[0][2 - j];
					this.bottom[0][2 - j] = this.left[2 - j][2];
					this.left[2 - j][2] = tempColor;
				}
				rotateCCW90(this.front);
				break;
			case Front180:
				for (int j = 0; j < 3; j++) {
					tempColor = this.top[2][j];
					this.top[2][j] = this.bottom[0][2 - j];
					this.bottom[0][2 - j] = tempColor;

					tempColor = this.right[j][0];
					this.right[j][0] = this.left[2 - j][2];
					this.left[2 - j][2] = tempColor;
				}
				rotate180(this.front);
				break;
			case LeftCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[j][0];
					this.front[j][0] = this.top[j][0];
					this.top[j][0] = this.back[2 - j][2];
					this.back[2 - j][2] = this.bottom[j][0];
					this.bottom[j][0] = tempColor;
				}
				rotateCW90(this.left);
				break;
			case LeftCCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[j][0];
					this.front[j][0] = this.bottom[j][0];
					this.bottom[j][0] = this.back[2 - j][2];
					this.back[2 - j][2] = this.top[j][0];
					this.top[j][0] = tempColor;
				}
				rotateCCW90(this.left);
				break;
			case Left180:
				for (int j = 0; j < 3; j++) {
					tempColor = this.top[j][0];
					this.top[j][0] = this.bottom[j][0];
					this.bottom[j][0] = tempColor;

					tempColor = this.front[j][0];
					this.front[j][0] = this.back[2 - j][2];
					this.back[2 - j][2] = tempColor;
				}
				rotate180(this.left);
				break;
			case RightCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[j][2];
					this.front[j][2] = this.bottom[j][2];
					this.bottom[j][2] = this.back[2 - j][0];
					this.back[2 - j][0] = this.top[j][2];
					this.top[j][2] = tempColor;
				}
				rotateCW90(this.right);
				break;
			case RightCCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[j][2];
					this.front[j][2] = this.top[j][2];
					this.top[j][2] = this.back[2 - j][0];
					this.back[2 - j][0] = this.bottom[j][2];
					this.bottom[j][2] = tempColor;
				}
				rotateCCW90(this.right);
				break;
			case Right180:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[j][2];
					this.front[j][2] = this.back[2 - j][0];
					this.back[2 - j][0] = tempColor;

					tempColor = this.bottom[j][2];
					this.bottom[j][2] = this.top[j][2];
					this.top[j][2] = tempColor;
				}
				rotate180(this.right);
				break;
			case TopCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[0][j];
					this.front[0][j] = this.right[0][j];
					this.right[0][j] = this.back[0][j];
					this.back[0][j] = this.left[0][j];
					this.left[0][j] = tempColor;
				}
				rotateCW90(this.top);
				break;
			case TopCCW90:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[0][j];
					this.front[0][j] = this.left[0][j];
					this.left[0][j] = this.back[0][j];
					this.back[0][j] = this.right[0][j];
					this.right[0][j] = tempColor;
				}
				rotateCCW90(this.top);
				break;
			case Top180:
				for (int j = 0; j < 3; j++) {
					tempColor = this.front[0][j];
					this.front[0][j] = this.back[0][j];
					this.back[0][j] = tempColor;

					tempColor = this.left[0][j];
					this.left[0][j] = this.right[0][j];
					this.right[0][j] = tempColor;
				}
				rotate180(this.top);
				break;
			case XCW90:
				tempFace = this.front;
				this.front = this.bottom;
				this.bottom = this.back;
				this.back = this.top;
				this.top = tempFace;

				rotateCW90(this.right);
				rotateCCW90(this.left);
				rotate180(this.back);
				rotate180(this.bottom);
				break;
			case XCCW90:
				tempFace = this.front;
				this.front = this.top;
				this.top = this.back;
				this.back = this.bottom;
				this.bottom = tempFace;

				rotateCCW90(this.right);
				rotateCW90(this.left);
				rotate180(this.top);
				rotate180(this.back);
				break;
			case X180:
				tempFace = this.front;
				this.front = this.back;
				this.back = tempFace;
				tempFace = this.top;
				this.top = this.bottom;
				this.bottom = tempFace;

				rotate180(this.right);
				rotate180(this.left);
				rotate180(this.back);
				rotate180(this.front);
				break;
			case YCW90:
				tempFace = this.front;
				this.front = this.left;
				this.left = this.back;
				this.back = this.right;
				this.right = tempFace;

				rotateCW90(this.top);
				rotateCW90(this.bottom);
				break;
			case YCCW90:
				tempFace = this.front;
				this.front = this.right;
				this.right = this.back;
				this.back = this.left;
				this.left = tempFace;

				rotateCCW90(this.top);
				rotateCCW90(this.bottom);
				break;
			case Y180:
				tempFace = this.front;
				this.front = this.back;
				this.back = tempFace;
				tempFace = this.right;
				this.right = this.left;
				this.left = tempFace;

				rotate180(this.top);
				rotate180(this.bottom);
				break;
			case ZCW90:
				break;
			case ZCCW90:
				break;
			case Z180:
				break;
			default:
				break;
			}
		}
	}

	public boolean isSolvedCube() {
		for (Color[][] face : cube.values()) {
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					if (face[y][x] != face[1][1])
						return false;
				}
			}
		}
		return true;
	}

	private boolean frblMatch(Color[] frblPattern) {
		Color[] target = { this.front[1][1], this.right[1][1], this.back[1][1], this.left[1][1], this.front[1][1],
				this.right[1][1], this.back[1][1], this.left[1][1] };

		for (int i = 0; i <= target.length - frblPattern.length; i++) {
			for (int j = 0; j < frblPattern.length; j++) {
				if (target[i + j] != frblPattern[j])
					break;
				if (j == frblPattern.length - 1)
					return true;
			}
		}
		return false;
	}

	private boolean validCenterpieces() {
		HashMap<Color, Integer> colorCounter = new HashMap<Color, Integer>();
		colorCounter.put(Color.Red, 0);
		colorCounter.put(Color.Orange, 0);
		colorCounter.put(Color.White, 0);
		colorCounter.put(Color.Yellow, 0);
		colorCounter.put(Color.Blue, 0);
		colorCounter.put(Color.Green, 0);

		colorCounter.put(this.front[1][1], colorCounter.get(this.front[1][1]) + 1);
		colorCounter.put(this.back[1][1], colorCounter.get(this.back[1][1]) + 1);
		colorCounter.put(this.left[1][1], colorCounter.get(this.left[1][1]) + 1);
		colorCounter.put(this.right[1][1], colorCounter.get(this.right[1][1]) + 1);
		colorCounter.put(this.top[1][1], colorCounter.get(this.top[1][1]) + 1);
		colorCounter.put(this.bottom[1][1], colorCounter.get(this.bottom[1][1]) + 1);
		for (int counter : colorCounter.values()) {
			if (counter != 1)
				return false;
		}

		if (this.top[1][1] == Color.Yellow) {
			if (this.bottom[1][1] != Color.White)
				return false;
			Color[] pattern = { Color.Red, Color.Green, Color.Orange, Color.Blue };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.White) {
			if (this.bottom[1][1] == Color.Yellow)
				return false;
			Color[] pattern = { Color.Red, Color.Blue, Color.Orange, Color.Green };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.Blue) {
			if (this.bottom[1][1] == Color.Green)
				return false;
			Color[] pattern = { Color.Red, Color.Yellow, Color.Orange, Color.White };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.Green) {
			if (this.bottom[1][1] == Color.Blue)
				return false;
			Color[] pattern = { Color.Red, Color.White, Color.Orange, Color.Yellow };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.Red) {
			if (this.bottom[1][1] == Color.Orange)
				return false;
			Color[] pattern = { Color.Yellow, Color.Blue, Color.White, Color.Green };
			return frblMatch(pattern);
		} else { // if (this.top[1][1] == Color.Orange)
			if (this.bottom[1][1] == Color.Red)
				return false;
			Color[] pattern = { Color.Yellow, Color.Green, Color.White, Color.Blue };
			return frblMatch(pattern);
		}
	}

	public boolean isValidCube() {
		if (!validCenterpieces())
			return false;

		return true;
	}

	public ArrayList<Move> solve() {
		ArrayList<Move> steps = new ArrayList<Move>();

		return steps;
	}
}