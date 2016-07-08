import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;


public class PriorityQueueTest {
	
	PriorityQueue<String> pQueue;

	@Before
	public void setUp() throws Exception {
		pQueue = new PriorityQueue<>();
		pQueue.addWithPriority("Hello", 9);
		pQueue.addWithPriority("World!", 9);
		pQueue.addWithPriority("Dogs", 5);
		pQueue.addWithPriority("Cats", 5);
		pQueue.addWithPriority("Capybaras", 7);
		pQueue.addWithPriority("Birds", 4);
		pQueue.addWithPriority("Rodents", 2);
		pQueue.addWithPriority("Insects", 1);
	}

	@Test
	public void testAddWithPriority() {
		pQueue.addWithPriority("Hello", 9);
		pQueue.addWithPriority("World!", 9);
		pQueue.addWithPriority("Dogs", 5);
		pQueue.addWithPriority("Cats", 5);
		pQueue.addWithPriority("Capybaras", 7);
		pQueue.addWithPriority("Birds", 4);
		pQueue.addWithPriority("Rodents", 2);
		pQueue.addWithPriority("Insects", 1);
	}
	
	@Test
	public void testRemove() {
		String word = pQueue.remove();
		String word2 = pQueue.remove();
		assertEquals("Hello", word);
		assertEquals("World!", word2);
		System.out.println(word + ", " + word2);

	}

}
