package queue;

import java.util.Arrays;
import java.util.NoSuchElementException;

public final class ArrayQueueMyTest {
    public static void main(final String[] args) {
        System.out.println("queue Test Suite:");

        queueTest();
        dequeTest();
    }

    public static void queueTest() {
        ArrayQueue queue = new ArrayQueue();

        System.out.println("Queue: enqueue test...");

        for (int i = 0; i < 5; ++i) {
            queue.enqueue("item#" + i);
        }

        assert queue.size() == 5;

        System.out.println("Queue: element test...");

        assert queue.element().equals("item#0");

        System.out.println("Queue: dequeue test...");

        assert queue.dequeue().equals("item#0");
        assert queue.size() == 4;
        assert queue.element().equals("item#1");

        System.out.println("Queue: clear test...");

        queue.clear();

        assert queue.size() == 0 && queue.isEmpty();

        System.out.println("Queue: clear dequeue test...");

        try {
            queue.dequeue();
            throw new AssertionError();
        }
        catch (NoSuchElementException ignored) {}
    }

    public static void dequeTest() {
        ArrayQueue deque = new ArrayQueue();

        System.out.println("Deque: push test...");

        for (int i = 0; i < 5; ++i) {
            deque.push("item#" + i);
        }

        assert deque.size() == 5;

        System.out.println("Deque: peek test...");

        assert deque.peek().equals("item#0");

        System.out.println("Deque: remove test...");

        assert deque.remove().equals("item#0");
        assert deque.size() == 4;
        assert deque.peek().equals("item#1");

        System.out.println("Deque: toArray test...");

        assert Arrays.equals(deque.toArray(), new Object[]{"item#4", "item#3", "item#2", "item#1"});

        deque.clear();

        try {
            deque.remove();
            throw new AssertionError();
        }
        catch (NoSuchElementException ignored) {}
    }
}
