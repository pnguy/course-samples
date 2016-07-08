import java.util.ArrayList;
import java.util.List;

public class PriorityQueue<E>{

	private List<E> queue;
	private List<Integer> priorityIndex;
	private int maxPriority;
	private int nextPriority;
	
	public PriorityQueue(){
		queue = new ArrayList<E>();
		priorityIndex = new ArrayList<Integer>();
		maxPriority = 0;
		nextPriority = 0;
		
	}
	
	public void addWithPriority(E element, int priority) {
		if (priority >= maxPriority)
			maxPriority = priority;
		if (priority < maxPriority && priority > nextPriority)
			nextPriority = priority;
		queue.add(element);
		priorityIndex.add(priority);
	}

	public E remove() {
		if (queue.size() == 0)
			return null;
		else{
			E remove = null;
			int i = 0;
			if ( priorityIndex.contains(maxPriority) == false){
				maxPriority = nextPriority;
				nextPriority = 0;
				for (int j = 0; j < priorityIndex.size(); j++){
					if (priorityIndex.get(j) < maxPriority && priorityIndex.get(j) >= nextPriority)
						nextPriority = priorityIndex.get(j);
				}
			}
			while( remove == null){
				if (priorityIndex.get(i) == maxPriority){
					remove = queue.remove(i);
					priorityIndex.remove(i);
					}
				i++;	
				}
			return remove;
		}
	}
	
}