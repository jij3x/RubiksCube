package rubikscube;

import java.util.ArrayList;
import java.util.HashMap;

public class EdgeBlockerCounter {
	private HashMap<EdgeBlocker, Integer> counter = new HashMap<EdgeBlocker, Integer>();

	public EdgeBlockerCounter() {
		ArrayList<Color> tuple = new ArrayList<Color>();
		tuple.add(Color.Yellow);
		tuple.add(Color.Red);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Green);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Orange);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Blue);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.clear();
		tuple.add(Color.White);
		tuple.add(Color.Red);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Green);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Orange);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Blue);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.clear();
		tuple.add(Color.Red);
		tuple.add(Color.Green);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Blue);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.clear();
		tuple.add(Color.Orange);
		tuple.add(Color.Green);
		counter.put(new EdgeBlocker(tuple), 0);

		tuple.remove(1);
		tuple.add(Color.Blue);
		counter.put(new EdgeBlocker(tuple), 0);
	}

	public boolean addEdgeBlocker(EdgeBlocker edgeBlocker) {
		if (!counter.containsKey(edgeBlocker) || counter.get(edgeBlocker) == 1)
			return false;

		counter.put(edgeBlocker, 1);
		return true;
	}

	public void clear() {
		for (EdgeBlocker edgeBlocker : counter.keySet())
			counter.put(edgeBlocker, 0);
	}
}
