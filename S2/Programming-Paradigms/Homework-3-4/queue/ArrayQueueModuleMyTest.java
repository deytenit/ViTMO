package queue;

import java.util.Arrays;
import java.util.NoSuchElementException;

public final class ArrayQueueModuleMyTest {
    public static void main(final String[] args) {
        System.out.println("ArrayQueueModule Test Suite:");

        queueTest();
        dequeTest();
    }

    public static void queueTest() {
        System.out.println("Queue: enqueue test...");

        for (int i = 0; i < 5; ++i) {
            ArrayQueueModule.enqueue("item#" + i);
        }

        assert ArrayQueueModule.size() == 5;

        System.out.println("Queue: element test...");

        assert ArrayQueueModule.element().equals("item#0");

        System.out.println("Queue: dequeue test...");

        assert ArrayQueueModule.dequeue().equals("item#0");
        assert ArrayQueueModule.size() == 4;
        assert ArrayQueueModule.element().equals("item#1");

        System.out.println("Queue: clear test...");

        ArrayQueueModule.clear();

        assert ArrayQueueModule.size() == 0 && ArrayQueueModule.isEmpty();


        System.out.println("Queue: clear dequeue test...");

        try {
            ArrayQueueModule.dequeue();
            throw new AssertionError();
        }
        catch (NoSuchElementException ignored) {}
    }

    public static void dequeTest() {
        System.out.println("Deque: push test...");

        for (int i = 0; i < 5; ++i) {
            ArrayQueueModule.push("item#" + i);
        }

        assert ArrayQueueModule.size() == 5;

        System.out.println("Deque: peek test...");

        assert ArrayQueueModule.peek().equals("item#0");

        System.out.println("Deque: remove test...");

        assert ArrayQueueModule.remove().equals("item#0");
        assert ArrayQueueModule.size() == 4;
        assert ArrayQueueModule.peek().equals("item#1");

        System.out.println("Deque: toArray test...");

        assert Arrays.equals(ArrayQueueModule.toArray(), new Object[]{"item#4", "item#3", "item#2", "item#1"});
    }
}
