package queue;

import java.util.Arrays;
import java.util.NoSuchElementException;

public final class ArrayQueueADTMyTest {
    public static void main(final String[] args) {
        System.out.println("ArrayQueueADT Test Suite:");

        queueTest();
        dequeTest();
    }

    public static void queueTest() {
        ArrayQueueADT queue = new ArrayQueueADT();
        
        System.out.println("Queue: enqueue test...");

        for (int i = 0; i < 5; ++i) {
            ArrayQueueADT.enqueue(queue, "item#" + i);
        }

        assert ArrayQueueADT.size(queue) == 5;

        System.out.println("Queue: element test...");

        assert ArrayQueueADT.element(queue).equals("item#0");

        System.out.println("Queue: dequeue test...");

        assert ArrayQueueADT.dequeue(queue).equals("item#0");
        assert ArrayQueueADT.size(queue) == 4;
        assert ArrayQueueADT.element(queue).equals("item#1");

        System.out.println("Queue: clear test...");

        ArrayQueueADT.clear(queue);

        assert ArrayQueueADT.size(queue) == 0 && ArrayQueueADT.isEmpty(queue);


        System.out.println("Queue: clear dequeue test...");

        try {
            ArrayQueueADT.dequeue(queue);
            throw new AssertionError();
        }
        catch (NoSuchElementException ignored) {}
    }

    public static void dequeTest() {
        ArrayQueueADT queue = new ArrayQueueADT();

        System.out.println("Deque: push test...");

        for (int i = 0; i < 5; ++i) {
            ArrayQueueADT.push(queue, "item#" + i);
        }

        assert ArrayQueueADT.size(queue) == 5;

        System.out.println("Deque: peek test...");

        assert ArrayQueueADT.peek(queue).equals("item#0");

        System.out.println("Deque: remove test...");

        assert ArrayQueueADT.remove(queue).equals("item#0");
        assert ArrayQueueADT.size(queue) == 4;
        assert ArrayQueueADT.peek(queue).equals("item#1");

        System.out.println("Deque: toArray test...");

        assert Arrays.equals(ArrayQueueADT.toArray(queue), new Object[]{"item#4", "item#3", "item#2", "item#1"});
    }
}
