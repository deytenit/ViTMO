package queue;

import java.util.NoSuchElementException;
import java.util.Objects;

// I:
// |    size >= 0
// |    && for 0 <= i < size -> data[i] != null
// |    && Assuming: 0 == head && size == tail
// immutable(l, r, a, a'):
// |    for l <= i < r -> a[i] == a'[i]
public class ArrayQueueADT {
    private Object[] data = new Object[2];
    private int head = 0;
    private int size = 0;

    private static Object[] toArray(final ArrayQueueADT that, final int capacity) {
        Object[] res = new Object[capacity];

        int pref = that.data.length - that.head;

        if (that.size <= pref) {
            System.arraycopy(that.data, that.head, res, 0, that.size);
        } else {
            System.arraycopy(that.data, that.head, res, 0, pref);
            System.arraycopy(that.data, 0, res, pref, that.size - pref);
        }

        return res;
    }

    private static void ensureCapacity(final ArrayQueueADT that, final int requiredCapacity) {
        if (that.data.length < requiredCapacity) {
            that.data = toArray(that,requiredCapacity * 2);
            that.head = 0;
        }
    }

    // Contract:
    // |    Pred:
    // |    |    that != null
    // |    Post:
    // |    |    size' = 0
    public static void clear(final ArrayQueueADT that) {
        Objects.requireNonNull(that);

        that.data = new Object[2];
        that.head = 0;
        that.size = 0;
    }

    // Contract:
    // |    Pred:
    // |    |   that != null
    // |    Post:
    // |    |   R == (size == 0)
    // |    |   && immutable(0, size, data, data')
    public static boolean isEmpty(final ArrayQueueADT that) {
        Objects.requireNonNull(that);

        return that.size == 0;
    }

    // Contract:
    // |    Pred:
    // |    |    that != null
    // |    Post:
    // |    |   R == size
    // |    |   && immutable(0, size, data, data')
    public static int size(final ArrayQueueADT that) {
        Objects.requireNonNull(that);

        return that.size;
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[head]
    // |    |   && immutable(0, size, data, data')
    public static Object element(final ArrayQueueADT that) {
        if (that.size == 0) {
            throw new NoSuchElementException("Cannot get element from an empty queue.");
        }

        return that.data[that.head];
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[size - 1]
    // |    |   && immutable(0, size, data, data')
    public static Object peek(final ArrayQueueADT that) {
        if (that.size == 0) {
            throw new NoSuchElementException("Cannot peek from an empty queue.");
        }

        return that.data[(that.head + that.size - 1) % that.data.length];
    }

    // Contract:
    // |    Pred:
    // |    |   that != null
    // |    |   && item != null
    // |    Post:
    // |    |   size' = size + 1
    // |    |   && data'[size] == item
    // |    |   && immutable(0, size, data, data')
    public static void enqueue(final ArrayQueueADT that, final Object item) {
        Objects.requireNonNull(that);
        Objects.requireNonNull(item);

        ensureCapacity(that, that.size + 1);

        that.data[(that.head + that.size) % that.data.length] = item;
        ++that.size;
    }

    // Contract:
    // |    Pred:
    // |    |   that != null
    // |    |   && item != null
    // |    Post:
    // |    |   size' = size + 1
    // |    |   && data'[0] == item
    // |    |   && immutable(1, size', data, data')
    public static void push(final ArrayQueueADT that, final Object item) {
        Objects.requireNonNull(item);
        Objects.requireNonNull(that);

        ensureCapacity(that, that.size + 1);
        if (that.head == 0) {
            that.head = that.data.length - 1;
        }
        else {
            --that.head;
        }

        that.data[that.head] = item;
        ++that.size;
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[0]
    // |    |   && immutable(1, size, data, data')
    // |    |   && size' = size - 1
    public static Object dequeue(final ArrayQueueADT that) {
        if (that.size == 0) {
            throw new NoSuchElementException("Cannot dequeue from an empty queue.");
        }

        final Object item = that.data[that.head];
        that.data[that.head] = null;

        that.head = (that.head + 1) % that.data.length;
        --that.size;

        return item;
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[size - 1]
    // |    |   && size' = size - 1
    // |    |   && immutable(0, size', data, data')
    public static Object remove(final ArrayQueueADT that) {
        if (that.size == 0) {
            throw new NoSuchElementException("Cannot remove from an empty queue.");
        }

        int tail = (that.head + that.size - 1) % that.data.length;

        final Object item = that.data[tail];
        that.data[tail] = null;

        --that.size;

        return item;
    }

    // Pre:
    // |    true
    // Post:
    // |    immutable(0, size, data, data')
    // |    R = data
    public static Object[] toArray(final ArrayQueueADT that) {
        if (that.size == 0) {
            return new Object[0];
        }

        return toArray(that, that.size);
    }
}
