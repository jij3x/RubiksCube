package rubikscube;

import java.util.ArrayList;
import java.util.Collections;

public class EdgeBlocker {
	public final ArrayList<Color> colorList;

	public EdgeBlocker() {
		colorList = new ArrayList<Color>();
	}

	public EdgeBlocker(ArrayList<Color> colorList) {
		this.colorList = new ArrayList<Color>(2);
		for (int i = 0; i < 2; i++) {
			this.colorList.add(colorList.get(i));
		}
		Collections.sort(this.colorList, new ColorComparator());
	}

	@Override
	public int hashCode() {
		int code = 0;
		for (int i = 0; i < this.colorList.size(); i++) {
			code = code * 10 + colorList.get(i).ordinal();
		}
		return code;
	}

	@Override
	public boolean equals(Object obj) {
		EdgeBlocker target = (EdgeBlocker) obj;
		for (int i = 0; i < this.colorList.size(); i++) {
			if (this.colorList.get(i) != target.colorList.get(i))
				return false;
		}
		return true;
	}
}
