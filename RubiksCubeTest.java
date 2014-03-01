public class RubiksCubeTest {

	private RubiksCube createCube() {
		HashMap<Color, Color[][]> newCube = new HashMap<Color, Color[][]>();
		Color[][] green = new Color[3][3];
		Color[][] blue = new Color[3][3];
		Color[][] red = new Color[3][3];
		Color[][] orange = new Color[3][3];
		Color[][] white = new Color[3][3];
		Color[][] yellow = new Color[3][3];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				green[i][j] = Color.Green;
				blue[i][j] = Color.Blue;
				red[i][j] = Color.Red;
				orange[i][j] = Color.Orange;
				white[i][j] = Color.White;
				yellow[i][j] = Color.Yellow;
			}
		}

		return new RubiksCube(newCube);
	}

	@Test
	public void test() {
		RubiksCube testingCube = createCube();
		assertTrue(testingCube.isSolvedCube());

		ArrayList<Move> steps = new ArrayList<Move>();
		steps.add(Move.RightClockwise);
		steps.add(Move.RightCounterCW);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.RightFlip);
		steps.add(Move.RightFlip);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.LeftClockwise);
		steps.add(Move.LeftCounterCW);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.LeftFlip);
		steps.add(Move.LeftFlip);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.FrontClockwise);
		steps.add(Move.FrontCounterCW);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.FrontFlip);
		steps.add(Move.FrontFlip);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.BackClockwise);
		steps.add(Move.BackCounterCW);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.BackFlip);
		steps.add(Move.BackFlip);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.TopClockwise);
		steps.add(Move.TopCounterCW);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.TopFlip);
		steps.add(Move.TopFlip);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.BottomClockwise);
		steps.add(Move.BottomCounterCW);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.BottomFlip);
		steps.add(Move.BottomFlip);
		assertTrue(testingCube.isSolvedCube());
	}
}
