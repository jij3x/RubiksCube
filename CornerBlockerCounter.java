package rubikscube;

import java.util.ArrayList;
import java.util.HashMap;

public class CornerBlockerCounter {
	private HashMap<CornerBlocker, Integer> counter = new HashMap<CornerBlocker, Integer>();

	public CornerBlockerCounter() {
		ArrayList<Color> triple = new ArrayList<Color>();
		triple.add(Color.Yellow);
		triple.add(Color.Red);
		triple.add(Color.Green);
		counter.put(new CornerBlocker(triple), 0);

		triple.clear();
		triple.add(Color.Yellow);
		triple.add(Color.Green);
		triple.add(Color.Orange);
		counter.put(new CornerBlocker(triple), 0);

		triple.clear();
		triple.add(Color.Yellow);
		triple.add(Color.Orange);
		triple.add(Color.Blue);
		counter.put(new CornerBlocker(triple), 0);

		triple.clear();
		triple.add(Color.Yellow);
		triple.add(Color.Red);
		triple.add(Color.Blue);
		counter.put(new CornerBlocker(triple), 0);

		triple.clear();
		triple.add(Color.White);
		triple.add(Color.Red);
		triple.add(Color.Green);
		counter.put(new CornerBlocker(triple), 0);

		triple.clear();
		triple.add(Color.White);
		triple.add(Color.Green);
		triple.add(Color.Orange);
		counter.put(new CornerBlocker(triple), 0);

		triple.clear();
		triple.add(Color.White);
		triple.add(Color.Orange);
		triple.add(Color.Blue);
		counter.put(new CornerBlocker(triple), 0);

		triple.clear();
		triple.add(Color.White);
		triple.add(Color.Red);
		triple.add(Color.Blue);
		counter.put(new CornerBlocker(triple), 0);
	}

	public boolean addCornerBlocker(CornerBlocker cornerBlocker) {
		if (!counter.containsKey(cornerBlocker) || counter.get(cornerBlocker) == 1)
			return false;

		counter.put(cornerBlocker, 1);
		return true;
	}

	public void clear() {
		for (CornerBlocker cornerBlocker : counter.keySet())
			counter.put(cornerBlocker, 0);
	}
}
