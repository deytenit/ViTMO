package queue;

import java.util.NoSuchElementException;
import java.util.Objects;

// I:
// |    size >= 0
// |    && for 0 <= i < size -> data[i] != null
// |    && Assuming: 0 == head && size == tail
// immutable(l, r, a, a'):
// |    for l <= i < r -> a[i] == a'[i]
public final class ArrayQueueModule {
    private static Object[] data = new Object[2];
    private static int head = 0;
    private static int size = 0;

    private static Object[] toArray(final int capacity) {
        Object[] res = new Object[capacity];

        int pref = data.length - head;

        if (size <= pref) {
            System.arraycopy(data, head, res, 0, size);
        } else {
            System.arraycopy(data, head, res, 0, pref);
            System.arraycopy(data, 0, res, pref, size - pref);
        }

        return res;
    }

    private static void ensureCapacity(final int requiredCapacity) {
        if (data.length < requiredCapacity) {
            data = toArray(requiredCapacity * 2);
            head = 0;
        }
    }

    // Contract:
    // |    Pred:
    // |    |    true
    // |    Post:
    // |    |    size' = 0
    public static void clear() {
        data = new Object[2];
        head = 0;
        size = 0;
    }

    // Contract:
    // |    Pred:
    // |    |   true
    // |    Post:
    // |    |   R == (size == 0)
    // |    |   && immutable(0, size, data, data')
    public static boolean isEmpty() {
        return size == 0;
    }

    // Contract:
    // |    Pred:
    // |    |    true
    // |    Post:
    // |    |   R == size
    // |    |   && immutable(0, size, data, data')
    public static int size() {
        return size;
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[head]
    // |    |   && immutable(0, size, data, data')
    public static Object element() {
        if (size == 0) {
            throw new NoSuchElementException("Cannot get element from an empty queue.");
        }

        return data[head];
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[size - 1]
    // |    |   && immutable(0, size, data, data')
    public static Object peek() {
        if (size == 0) {
            throw new NoSuchElementException("Cannot peek from an empty queue.");
        }

        return data[(head + size - 1) % data.length];
    }

    // Contract:
    // |    Pred:
    // |    |   item != null
    // |    Post:
    // |    |   size' = size + 1
    // |    |   && data'[size] == item
    // |    |   && immutable(0, size, data, data')
    public static void enqueue(final Object item) {
        Objects.requireNonNull(item);

        ensureCapacity(size + 1);

        data[(head + size) % data.length] = item;
        ++size;
    }

    // Contract:
    // |    Pred:
    // |    |   item != null
    // |    Post:
    // |    |   size' = size + 1
    // |    |   && data'[0] == item
    // |    |   && immutable(1, size', data, data')
    public static void push(final Object item) {
        Objects.requireNonNull(item);

        ensureCapacity(size + 1);

        if (head == 0) {
            head = data.length - 1;
        }
        else {
            --head;
        }

        data[head] = item;
        ++size;
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[0]
    // |    |   && immutable(1, size, data, data')
    // |    |   && size' = size - 1
    public static Object dequeue() {
        if (size == 0) {
            throw new NoSuchElementException("Cannot dequeue from an empty queue.");
        }

        final Object item = data[head];
        data[head] = null;

        head = (head + 1) % data.length;
        --size;

        return item;
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[size - 1]
    // |    |   && size' = size - 1
    // |    |   && immutable(0, size', data, data')
    public static Object remove() {
        if (size == 0) {
            throw new NoSuchElementException("Cannot remove from an empty queue.");
        }

        int tail = (head + size - 1) % data.length;

        final Object item = data[tail];
        data[tail] = null;

        --size;

        return item;
    }

    // Pre:
    // |    true
    // Post:
    // |    immutable(0, size, data, data')
    // |    R = data
    public static Object[] toArray() {
        if (size == 0) {
            return new Object[0];
        }

        return toArray(size);
    }
}
