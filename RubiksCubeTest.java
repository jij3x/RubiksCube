package rubikscube;

import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

import org.junit.Test;

public class RubiksCubeTest {
	@Test
	public void testSimpleMove() {
		RubiksCube testingCube = new RubiksCube();
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

	@Test
	public void testNormalMove() {
		ArrayList<Move> steps = new ArrayList<Move>();
		steps.add(Move.RightCW90);
		steps.add(Move.LeftCW90);
		steps.add(Move.TopCW90);
		steps.add(Move.BottomCW90);
		steps.add(Move.Front180);
		steps.add(Move.LeftCW90);
		steps.add(Move.TopCW90);
		steps.add(Move.Back180);
		steps.add(Move.Right180);
		steps.add(Move.Bottom180);
		steps.add(Move.LeftCCW90);
		steps.add(Move.TopCCW90);

		RubiksCube referenceCube = new RubiksCube();
		referenceCube.turnCube(steps);
		assertTrue(referenceCube.isValidCube());

		RubiksCube testingCube = new RubiksCube();
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.RightCW90);
		steps.add(Move.RightCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Right180);
		steps.add(Move.Right180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.LeftCW90);
		steps.add(Move.LeftCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Left180);
		steps.add(Move.Left180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.FrontCW90);
		steps.add(Move.FrontCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Front180);
		steps.add(Move.Front180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.BackCW90);
		steps.add(Move.BackCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Back180);
		steps.add(Move.Back180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.TopCW90);
		steps.add(Move.TopCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Top180);
		steps.add(Move.Top180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.BottomCW90);
		steps.add(Move.BottomCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Bottom180);
		steps.add(Move.Bottom180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.XCW90);
		steps.add(Move.XCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.X180);
		steps.add(Move.X180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.YCW90);
		steps.add(Move.YCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Y180);
		steps.add(Move.Y180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));

		steps.clear();
		steps.add(Move.ZCW90);
		steps.add(Move.ZCCW90);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
		steps.clear();
		steps.add(Move.Z180);
		steps.add(Move.Z180);
		testingCube.turnCube(steps);
		assertTrue(testingCube.isSameCube(referenceCube));
	}

	private ArrayList<Move> reverse(ArrayList<Move> steps) {
		ArrayList<Move> reverseSteps = new ArrayList<Move>();
		for (int i = steps.size() - 1; i >= 0; i--) {
			switch (steps.get(i)) {
			case Back180:
				reverseSteps.add(Move.Back180);
				break;
			case BackCCW90:
				reverseSteps.add(Move.BackCW90);
				break;
			case BackCW90:
				reverseSteps.add(Move.BackCCW90);
				break;
			case Bottom180:
				reverseSteps.add(Move.Bottom180);
				break;
			case BottomCCW90:
				reverseSteps.add(Move.BottomCW90);
				break;
			case BottomCW90:
				reverseSteps.add(Move.BottomCCW90);
				break;
			case Front180:
				reverseSteps.add(Move.Front180);
				break;
			case FrontCCW90:
				reverseSteps.add(Move.FrontCW90);
				break;
			case FrontCW90:
				reverseSteps.add(Move.FrontCCW90);
				break;
			case Left180:
				reverseSteps.add(Move.Left180);
				break;
			case LeftCCW90:
				reverseSteps.add(Move.LeftCW90);
				break;
			case LeftCW90:
				reverseSteps.add(Move.LeftCCW90);
				break;
			case Right180:
				reverseSteps.add(Move.Right180);
				break;
			case RightCCW90:
				reverseSteps.add(Move.RightCW90);
				break;
			case RightCW90:
				reverseSteps.add(Move.RightCCW90);
				break;
			case Top180:
				reverseSteps.add(Move.Top180);
				break;
			case TopCCW90:
				reverseSteps.add(Move.TopCW90);
				break;
			case TopCW90:
				reverseSteps.add(Move.TopCCW90);
				break;
			case X180:
				reverseSteps.add(Move.X180);
				break;
			case XCCW90:
				reverseSteps.add(Move.XCW90);
				break;
			case XCW90:
				reverseSteps.add(Move.XCCW90);
				break;
			case Y180:
				reverseSteps.add(Move.Y180);
				break;
			case YCCW90:
				reverseSteps.add(Move.YCW90);
				break;
			case YCW90:
				reverseSteps.add(Move.YCCW90);
				break;
			case Z180:
				reverseSteps.add(Move.Z180);
				break;
			case ZCCW90:
				reverseSteps.add(Move.ZCW90);
				break;
			case ZCW90:
				reverseSteps.add(Move.ZCCW90);
				break;
			default:
				break;
			}
		}
		return reverseSteps;
	}

	@Test
	public void testExhaustiveMove() {
		ArrayList<Move> steps = new ArrayList<Move>();
		for (Move move : Move.values()) {
			for (int i = 0; i < 1024; i++)
				steps.add(move);
		}

		long seed = System.nanoTime();
		Collections.shuffle(steps, new Random(seed));
		Collections.shuffle(steps, new Random(seed));

		RubiksCube testingCube = new RubiksCube();

		Collections.shuffle(steps, new Random(seed));
		Collections.shuffle(steps, new Random(seed));

		testingCube.turnCube(steps);
		testingCube.turnCube(reverse(steps));
		assertTrue(testingCube.isSolvedCube());
	}
}
