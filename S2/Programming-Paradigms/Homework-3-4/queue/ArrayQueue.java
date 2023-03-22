package queue;

import java.util.*;

public class ArrayQueue extends AbstractQueue {
    private Object[] data;
    private int head;

    private Object[] toArray(final int capacity) {
        final Object[] res = new Object[capacity];

        final int pref = data.length - head;

        if (size <= pref) {
            System.arraycopy(data, head, res, 0, size);
        } else {
            System.arraycopy(data, head, res, 0, pref);
            System.arraycopy(data, 0, res, pref, size - pref);
        }

        return res;
    }

    private void ensureCapacity(final int requiredCapacity) {
        if (data.length < requiredCapacity) {
            data = toArray(requiredCapacity * 2);
            head = 0;
        }
    }

    @Override
    public void clear() {
        data = new Object[2];
        head = 0;
        size = 0;
    }

    @Override
    protected Object elementImpl() {
        return data[head];
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[size - 1]
    // |    |   && immutable(0, size, data, data')
    public Object peek() {
        if (size == 0) {
            throw new NoSuchElementException("Cannot peek from an empty queue.");
        }

        return data[(head + size - 1) % data.length];
    }

    @Override
    protected void enqueueImpl(final Object item) {
        ensureCapacity(size + 1);

        data[(head + size) % data.length] = item;
    }

    // Contract:
    // |    Pred:
    // |    |   item != null
    // |    Post:
    // |    |   size' = size + 1
    // |    |   && data'[0] == item
    // |    |   && immutable(1, size', data, data')
    public void push(final Object item) {
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

    @Override
    protected Object dequeueImpl() {
        final Object item = data[head];
        data[head] = null;

        head = (head + 1) % data.length;

        return item;
    }

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[size - 1]
    // |    |   && size' = size - 1
    // |    |   && immutable(0, size', data, data')
    public Object remove() {
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
    public Object[] toArray() {
        if (size == 0) {
            return new Object[0];
        }

        return toArray(size);
    }
}
