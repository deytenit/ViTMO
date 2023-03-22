package queue;

import java.util.*;

public abstract class AbstractQueue implements Queue {
    protected int size;

    public AbstractQueue() {
        clear();
    }

    @Override
    public final boolean isEmpty() {
        return size == 0;
    }

    @Override
    public final int size() {
        return size;
    }

    @Override
    public final Object element() {
        if (size == 0) {
            throw new NoSuchElementException("Cannot get element from an empty queue.");
        }

        return elementImpl();
    }

    @Override
    public final void enqueue(final Object item) {
        Objects.requireNonNull(item);

        enqueueImpl(item);
        ++size;
    }

    @Override
    public final Object dequeue() {
        if (size == 0) {
            throw new NoSuchElementException("Cannot dequeue from an empty queue.");
        }

        final Object result = dequeueImpl();
        --size;

        return result;
    }

    @Override
    public final boolean contains(final Object query) {
        Objects.requireNonNull(query);

        return containsRemove(query, false);
    }

    @Override
    public final boolean removeFirstOccurrence(final Object query) {
        Objects.requireNonNull(query);

        return containsRemove(query, true);
    }

    private boolean containsRemove(final Object query, final boolean shouldRemove) {
        boolean result = false;

        for (int i = 0; i < size; ++i) {
            final Object item = dequeue();

            if (!result && item.equals(query)) {
                result = true;

                if (shouldRemove) {
                    --i;
                    continue;
                }
            }

            enqueue(item);
        }

        return result;
    }

    protected abstract Object elementImpl();

    protected abstract void enqueueImpl(final Object item);

    protected abstract Object dequeueImpl();
}
