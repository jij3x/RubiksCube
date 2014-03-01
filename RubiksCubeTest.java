package rubikscube;

import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.HashMap;

import org.junit.Test;

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

		newCube.put(Color.Green, green);
		newCube.put(Color.Blue, blue);
		newCube.put(Color.Red, red);
		newCube.put(Color.Orange, orange);
		newCube.put(Color.White, white);
		newCube.put(Color.Yellow, yellow);

		return new RubiksCube(newCube);
	}

	@Test
	public void test() {
		RubiksCube testingCube = createCube();
		assertTrue(testingCube.isSolvedCube());

		ArrayList<Move> steps = new ArrayList<Move>();
		steps.add(Move.RightCW90);
		steps.add(Move.RightCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.Right180);
		steps.add(Move.Right180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.LeftCW90);
		steps.add(Move.LeftCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.Left180);
		steps.add(Move.Left180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.FrontCW90);
		steps.add(Move.FrontCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.Front180);
		steps.add(Move.Front180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.BackCW90);
		steps.add(Move.BackCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.Back180);
		steps.add(Move.Back180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.TopCW90);
		steps.add(Move.TopCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.Top180);
		steps.add(Move.Top180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());

		steps.clear();
		steps.add(Move.BottomCW90);
		steps.add(Move.BottomCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());
		steps.clear();
		steps.add(Move.Bottom180);
		steps.add(Move.Bottom180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSolvedCube());
	}
}
