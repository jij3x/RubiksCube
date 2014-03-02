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

	private HashMap<Color, Color[][]> clone(HashMap<Color, Color[][]> sourceCube) {
		HashMap<Color, Color[][]> cloneCube = new HashMap<Color, Color[][]>(sourceCube.size());
		for (Color[][] face : sourceCube.values()) {
			Color[][] cloneFace = new Color[face.length][face[0].length];
			for (int y = 0; y < cloneFace.length; y++) {
				for (int x = 0; x < cloneFace[0].length; x++) {
					cloneFace[y][x] = face[y][x];
				}
			}
			cloneCube.put(cloneFace[1][1], cloneFace);
		}
		return cloneCube;
	}

	public RubiksCube() {
		this.cube = new HashMap<Color, Color[][]>();

		this.cube.put(Color.Green, new Color[3][3]);
		this.cube.put(Color.Blue, new Color[3][3]);
		this.cube.put(Color.Red, new Color[3][3]);
		this.cube.put(Color.Orange, new Color[3][3]);
		this.cube.put(Color.White, new Color[3][3]);
		this.cube.put(Color.Yellow, new Color[3][3]);

		for (Color k : this.cube.keySet()) {
			Color[][] face = this.cube.get(k);
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++)
					face[y][x] = k;
			}
		}

		this.top = cube.get(Color.Yellow);
		this.bottom = cube.get(Color.White);
		this.left = cube.get(Color.Blue);
		this.right = cube.get(Color.Green);
		this.front = cube.get(Color.Red);
		this.back = cube.get(Color.Orange);
	}

	public RubiksCube(RubiksCube targetCube) {
		this(targetCube.getCubeSnapshot());
	}

	public RubiksCube(HashMap<Color, Color[][]> cube) {
		this.cube = clone(cube);

		this.top = cube.get(Color.Yellow);
		this.bottom = cube.get(Color.White);
		this.left = cube.get(Color.Blue);
		this.right = cube.get(Color.Green);
		this.front = cube.get(Color.Red);
		this.back = cube.get(Color.Orange);
	}

	public final HashMap<Color, Color[][]> getCubeSnapshot() {
		return clone(cube);
	}

	private void rotateCW90(Color[][] face) {
		for (int i = 0; i < 2; i++) {
			Color temp = face[0][i];
			face[0][i] = face[2 - i][0];
			face[2 - i][0] = face[2][2 - i];
			face[2][2 - i] = face[i][2];
			face[i][2] = temp;
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
					this.left[2 - j][2] = this.bottom[0][2 - j];
					this.bottom[0][2 - j] = this.right[j][0];
					this.right[j][0] = tempColor;
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
				this.front = this.right;
				this.right = this.back;
				this.back = this.left;
				this.left = tempFace;

				rotateCW90(this.top);
				rotateCCW90(this.bottom);
				break;
			case YCCW90:
				tempFace = this.front;
				this.front = this.left;
				this.left = this.back;
				this.back = this.right;
				this.right = tempFace;

				rotateCCW90(this.top);
				rotateCW90(this.bottom);
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
				tempFace = this.top;
				this.top = this.left;
				this.left = this.bottom;
				this.bottom = this.right;
				this.right = tempFace;

				rotateCW90(this.front);
				rotateCCW90(this.back);
				rotateCW90(this.right);
				rotateCW90(this.top);
				rotateCW90(this.left);
				rotateCW90(this.bottom);
				break;
			case ZCCW90:
				tempFace = this.top;
				this.top = this.right;
				this.right = this.bottom;
				this.bottom = this.left;
				this.left = tempFace;

				rotateCCW90(this.front);
				rotateCW90(this.back);
				rotateCCW90(this.right);
				rotateCCW90(this.top);
				rotateCCW90(this.left);
				rotateCCW90(this.bottom);
				break;
			case Z180:
				tempFace = this.top;
				this.top = this.bottom;
				this.bottom = tempFace;
				tempFace = this.right;
				this.right = this.left;
				this.left = tempFace;

				rotate180(this.front);
				rotate180(this.back);
				rotate180(this.top);
				rotate180(this.bottom);
				rotate180(this.left);
				rotate180(this.right);
				break;
			default:
				break;
			}
		}
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
		if (this.top[1][1] == Color.Yellow) {
			if (this.bottom[1][1] != Color.White)
				return false;
			Color[] pattern = { Color.Red, Color.Green, Color.Orange, Color.Blue };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.White) {
			if (this.bottom[1][1] != Color.Yellow)
				return false;
			Color[] pattern = { Color.Red, Color.Blue, Color.Orange, Color.Green };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.Blue) {
			if (this.bottom[1][1] != Color.Green)
				return false;
			Color[] pattern = { Color.Red, Color.Yellow, Color.Orange, Color.White };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.Green) {
			if (this.bottom[1][1] != Color.Blue)
				return false;
			Color[] pattern = { Color.Red, Color.White, Color.Orange, Color.Yellow };
			return frblMatch(pattern);
		} else if (this.top[1][1] == Color.Red) {
			if (this.bottom[1][1] != Color.Orange)
				return false;
			Color[] pattern = { Color.Yellow, Color.Blue, Color.White, Color.Green };
			return frblMatch(pattern);
		} else { // if (this.top[1][1] == Color.Orange)
			if (this.bottom[1][1] != Color.Red)
				return false;
			Color[] pattern = { Color.Yellow, Color.Green, Color.White, Color.Blue };
			return frblMatch(pattern);
		}
	}

	private boolean validEdgePieces() {
		EdgeBlockerCounter counter = new EdgeBlockerCounter();

		ArrayList<Color> tuple = new ArrayList<Color>();
		tuple.add(this.top[2][1]);
		tuple.add(this.front[0][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.top[1][2]);
		tuple.add(this.right[0][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.top[1][0]);
		tuple.add(this.left[0][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.top[0][1]);
		tuple.add(this.back[0][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.bottom[0][1]);
		tuple.add(this.front[2][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.bottom[1][2]);
		tuple.add(this.right[2][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.bottom[1][0]);
		tuple.add(this.left[2][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.bottom[2][1]);
		tuple.add(this.back[2][1]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.front[1][2]);
		tuple.add(this.right[1][0]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.front[1][0]);
		tuple.add(this.left[1][2]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.right[1][2]);
		tuple.add(this.back[1][0]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		tuple.clear();
		tuple.add(this.left[1][0]);
		tuple.add(this.back[1][2]);
		if (!counter.addEdgeBlocker(new EdgeBlocker(tuple)))
			return false;

		return true;
	}

	public boolean valideCornerPieces() {
		CornerBlockerCounter counter = new CornerBlockerCounter();

		ArrayList<Color> triple = new ArrayList<Color>();
		triple.add(this.top[2][2]);
		triple.add(this.front[0][2]);
		triple.add(this.right[0][0]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		triple.clear();
		triple.add(this.top[0][2]);
		triple.add(this.right[0][2]);
		triple.add(this.back[0][0]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		triple.clear();
		triple.add(this.top[0][0]);
		triple.add(this.back[0][2]);
		triple.add(this.left[0][0]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		triple.clear();
		triple.add(this.top[2][0]);
		triple.add(this.front[0][0]);
		triple.add(this.left[0][2]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		triple.clear();
		triple.add(this.bottom[0][0]);
		triple.add(this.front[2][0]);
		triple.add(this.left[2][2]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		triple.clear();
		triple.add(this.bottom[0][2]);
		triple.add(this.front[2][2]);
		triple.add(this.right[2][0]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		triple.clear();
		triple.add(this.bottom[2][2]);
		triple.add(this.right[2][2]);
		triple.add(this.back[2][0]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		triple.clear();
		triple.add(this.bottom[2][0]);
		triple.add(this.back[2][2]);
		triple.add(this.left[2][0]);
		if (!counter.addCornerBlocker(new CornerBlocker(triple)))
			return false;

		return true;
	}

	public boolean orientFaces() {

		return true;
	}

	public boolean isValidCube() {
		if (!validCenterpieces())
			return false;
		if (!validEdgePieces())
			return false;
		if (!valideCornerPieces())
			return false;
		if (!orientFaces())
			return false;

		return true;
	}

	public boolean isSameCube(RubiksCube target) {
		HashMap<Color, Color[][]> targetCube = target.getCubeSnapshot();

		for (Color k : this.cube.keySet()) {
			if (!targetCube.containsKey(k))
				return false;

			Color[][] face = this.cube.get(k);
			Color[][] targetFace = targetCube.get(k);
			for (int i = 0; i < face.length; i++) {
				for (int j = 0; j < face[0].length; j++) {
					if (face[i][j] != targetFace[i][j])
						return false;
				}
			}
		}
		return true;
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

	public ArrayList<Move> solve() {
		ArrayList<Move> steps = new ArrayList<Move>();

		return steps;
	}
}