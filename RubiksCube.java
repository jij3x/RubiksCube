public class RubiksCube {
	/*
	 * +---+
	 * | Y |
	 * +---+---+---+---+
	 * | R | G | O | B |
	 * +---+---+---+---+
	 * | W |
	 * +---+
	 */
	public enum Color {
		White, Yellow, Blue, Green, Red, Orange
	};

	public enum Move {
		RightClockwise, RightFlip, RightCounterCW, LeftClockwise, LeftFlip, LeftCounterCW, FrontClockwise, FrontFlip,
		FrontCounterCW, BackClockwise, BackFlip, BackCounterCW, BottomClockwise, BottomFlip, BottomCounterCW,
		TopClockwise, TopFlip, TopCounterCW
	};

	private HashMap<Color, Color[][]> cube;

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
	}

	public void clockwiseRotate(Color[][] face) {
		for (int i = 0; i < 2; i++) {
			Color temp = face[0][i];
			face[0][i] = face[2 - i][0];
			face[2 - i][0] = face[2][2 - i];
			face[2][2 - i] = face[2 - i][2];
			face[2 - i][2] = temp;
		}
	}

	public void counterCWRotate(Color[][] face) {
		for (int i = 0; i < 2; i++) {
			Color temp = face[0][i];
			face[0][i] = face[i][2];
			face[i][2] = face[2][2 - i];
			face[2][2 - i] = face[2 - i][0];
			face[2 - i][0] = temp;
		}
	}

	public void flipRotate(Color[][] face) {
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
		turnCube(cube.get(Color.Yellow), cube.get(Color.White), cube.get(Color.Blue), cube.get(Color.Green),
				cube.get(Color.Red), cube.get(Color.Orange), steps);
	}

	public void turnCube(Color[][] top, Color[][] bottom, Color[][] left, Color[][] right, Color[][] front,
			Color[][] back, ArrayList<Move> steps) {
		Color temp;
		for (int idx = 0; idx < steps.size(); idx++) {
			switch (steps.get(idx)) {
			case BackClockwise:
				for (int j = 0; j < 3; j++) {
					temp = right[j][2];
					right[j][2] = bottom[2][2 - j];
					bottom[2][2 - j] = left[2 - j][0];
					left[2 - j][0] = top[0][j];
					top[0][j] = temp;
				}
				clockwiseRotate(back);
				break;
			case BackCounterCW:
				for (int j = 0; j < 3; j++) {
					temp = right[j][2];
					right[j][2] = top[0][j];
					top[0][j] = left[2 - j][0];
					left[2 - j][0] = bottom[2][2 - j];
					bottom[2][2 - j] = temp;
				}
				counterCWRotate(back);
				break;
			case BackFlip:
				for (int j = 0; j < 3; j++) {
					temp = right[j][2];
					right[j][2] = left[2 - j][0];
					left[2 - j][0] = temp;

					temp = top[0][j];
					top[0][j] = bottom[2][2 - j];
					bottom[2][2 - j] = temp;
				}
				flipRotate(back);
				break;
			case BottomClockwise:
				for (int j = 0; j < 3; j++) {
					temp = front[2][j];
					front[2][j] = left[2][j];
					left[2][j] = back[2][j];
					back[2][j] = right[2][j];
					right[2][j] = temp;
				}
				clockwiseRotate(bottom);
				break;
			case BottomCounterCW:
				for (int j = 0; j < 3; j++) {
					temp = front[2][j];
					front[2][j] = right[2][j];
					right[2][j] = back[2][j];
					back[2][j] = left[2][j];
					left[2][j] = temp;
				}
				counterCWRotate(bottom);
				break;
			case BottomFlip:
				for (int j = 0; j < 3; j++) {
					temp = front[2][j];
					front[2][j] = back[2][j];
					back[2][j] = temp;

					temp = left[2][j];
					left[2][j] = right[2][j];
					right[2][j] = temp;
				}
				flipRotate(bottom);
				break;
			case FrontClockwise:
				for (int j = 0; j < 3; j++) {
					temp = top[2][j];
					top[2][j] = left[2 - j][2];
					left[2 - j][2] = bottom[0][j];
					bottom[0][j] = right[2 - j][0];
					right[2 - j][0] = temp;
				}
				clockwiseRotate(front);
				break;
			case FrontCounterCW:
				for (int j = 0; j < 3; j++) {
					temp = top[2][j];
					top[2][j] = right[j][0];
					right[j][0] = bottom[0][2 - j];
					bottom[0][2 - j] = left[2 - j][2];
					left[2 - j][2] = temp;
				}
				counterCWRotate(front);
				break;
			case FrontFlip:
				for (int j = 0; j < 3; j++) {
					temp = top[2][j];
					top[2][j] = bottom[0][2 - j];
					bottom[0][2 - j] = temp;

					temp = right[j][0];
					right[j][0] = left[2 - j][2];
					left[2 - j][2] = temp;
				}
				flipRotate(front);
				break;
			case LeftClockwise:
				for (int j = 0; j < 3; j++) {
					temp = front[j][0];
					front[j][0] = top[j][0];
					top[j][0] = back[2 - j][2];
					back[2 - j][2] = bottom[j][0];
					bottom[j][0] = temp;
				}
				clockwiseRotate(left);
				break;
			case LeftCounterCW:
				for (int j = 0; j < 3; j++) {
					temp = front[j][0];
					front[j][0] = bottom[j][0];
					bottom[j][0] = back[2 - j][2];
					back[2 - j][2] = top[j][0];
					top[j][0] = temp;
				}
				counterCWRotate(left);
				break;
			case LeftFlip:
				for (int j = 0; j < 3; j++) {
					temp = top[j][0];
					top[j][0] = bottom[j][0];
					bottom[j][0] = temp;

					temp = front[j][0];
					front[j][0] = back[2 - j][2];
					back[2 - j][2] = temp;
				}
				flipRotate(left);
				break;
			case RightClockwise:
				for (int j = 0; j < 3; j++) {
					temp = front[j][2];
					front[j][2] = bottom[j][2];
					bottom[j][2] = back[2 - j][0];
					back[2 - j][0] = top[j][2];
					top[j][2] = temp;
				}
				clockwiseRotate(right);
				break;
			case RightCounterCW:
				for (int j = 0; j < 3; j++) {
					temp = front[j][2];
					front[j][2] = top[j][2];
					top[j][2] = back[2 - j][0];
					back[2 - j][0] = bottom[j][2];
					bottom[j][2] = temp;
				}
				counterCWRotate(right);
				break;
			case RightFlip:
				for (int j = 0; j < 3; j++) {
					temp = front[j][2];
					front[j][2] = back[2 - j][0];
					back[2 - j][0] = temp;

					temp = bottom[j][2];
					bottom[j][2] = top[j][2];
					top[j][2] = temp;
				}
				flipRotate(right);
				break;
			case TopClockwise:
				for (int j = 0; j < 3; j++) {
					temp = front[0][j];
					front[0][j] = right[0][j];
					right[0][j] = back[0][j];
					back[0][j] = left[0][j];
					left[0][j] = temp;
				}
				clockwiseRotate(top);
				break;
			case TopCounterCW:
				for (int j = 0; j < 3; j++) {
					temp = front[0][j];
					front[0][j] = left[0][j];
					left[0][j] = back[0][j];
					back[0][j] = right[0][j];
					right[0][j] = temp;
				}
				counterCWRotate(top);
				break;
			case TopFlip:
				for (int j = 0; j < 3; j++) {
					temp = front[0][j];
					front[0][j] = back[0][j];
					back[0][j] = temp;

					temp = left[0][j];
					left[0][j] = right[0][j];
					right[0][j] = temp;
				}
				flipRotate(top);
				break;
			default:
				break;
			}
		}
	}

	public ArrayList<Move> solve() {
		ArrayList<Move> steps = new ArrayList<Move>();

		return steps;
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
}
