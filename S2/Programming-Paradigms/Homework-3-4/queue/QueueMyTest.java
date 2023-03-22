package queue;

import java.util.NoSuchElementException;

public final class QueueMyTest {
    public static void main(final String[] args) {
        System.out.println("ArrayQueue Test Suite:");
        queueTest(new ArrayQueue());
        System.out.println("LinkedQueue Test Suite:");
        queueTest(new LinkedQueue());
    }

    public static void queueTest(final Queue queue) {
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

        System.out.println("Queue: contains test...");

        assert queue.contains("item#1");
        assert queue.contains("item#3");

        System.out.println("Queue: removeFirstOccurrence test...");

        assert queue.removeFirstOccurrence("item#2");
        assert !queue.contains("item#2");

        assert queue.removeFirstOccurrence("item#1");
        assert queue.element().equals("item#3");

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
}

