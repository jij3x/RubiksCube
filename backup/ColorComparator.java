package rubikscube;

import java.util.Comparator;

public class ColorComparator implements Comparator<Color> {
	@Override
	public int compare(Color o1, Color o2) {
		return o1.ordinal() - o2.ordinal();
	}
}
