public class RubiksCubeTest {

	@Test
	public void test() {
		HashMap<Color, Color[][]> origCube = new HashMap<Color, Color[][]>();
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

		RubiksCube testingCube = new RubiksCube(origCube);
		assertTrue("", testingCube.isSolvedCube());

	}
}
