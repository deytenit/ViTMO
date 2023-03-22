package queue;

public class LinkedQueue extends AbstractQueue {
    private Node head;
    private Node tail;

    @Override
    public void clear() {
        head = null;
        tail = null;
        size = 0;
    }

    @Override
    protected Object elementImpl() {
        return head.item;
    }

    @Override
    protected void enqueueImpl(final Object item) {
        if (size == 0) {
            head = tail = new Node(item);
        }
        else {
            final Node tmp = new Node(item);
            tail.next = tmp;
            tail = tmp;
        }
    }

    @Override
    protected Object dequeueImpl() {
        final Object result = head.item;

        head = head.next;

        return result;
    }

    private static class Node {
        private final Object item;

        private Node next;

        public Node(final Object item) {
            this.item = item;
            this.next = null;
        }
    }
}
