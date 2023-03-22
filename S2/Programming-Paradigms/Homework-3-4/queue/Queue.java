package queue;

// I:
// |    size >= 0
// |    && for 0 <= i < size -> data[i] != null
// |    && Assuming: 0 == head && size == tail
// immutable(l, r, a, a'):
// |    for l <= i < r -> a[i] == a'[i]
public interface Queue {
    // Contract:
    // |    Pred:
    // |    |    true
    // |    Post:
    // |    |    size' = 0
    void clear();

    // Contract:
    // |    Pred:
    // |    |   true
    // |    Post:
    // |    |   R == (size == 0)
    // |    |   && immutable(0, size, data, data')
    boolean isEmpty();

    // Contract:
    // |    Pred:
    // |    |    true
    // |    Post:
    // |    |   R == size
    // |    |   && immutable(0, size, data, data')
    int size();

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[head]
    // |    |   && immutable(0, size, data, data')
    Object element();

    // Contract:
    // |    Pred:
    // |    |   item != null
    // |    Post:
    // |    |   size' = size + 1
    // |    |   && data'[size] == item
    // |    |   && immutable(0, size, data, data')
    void enqueue(final Object item);

    // Contract:
    // |    Pred:
    // |    |   size > 0
    // |    Post:
    // |    |   R == data[0]
    // |    |   && immutable(1, size, data, data')
    // |    |   && size' = size - 1
    Object dequeue();

    // Contract:
    // |    Pred:
    // |    |   query != null
    // |    Post:
    // |    |   R == (if exists i: 0 <= i < size && data[i] == query)
    // |    |   && immutable(0, size, data, data')
    boolean contains(final Object query);

    // Contract:
    // |    Pred:
    // |    |   query != null
    // |    Post:
    // |    |   R == (if exists i: 0 <= i < size && data[i] == query)
    // |    |   && R ? immutable(0, i, data, data') && immutable(i, size', data, data')
    // |    |       : immutable(0, size, data, data')
    // |    |   && R ? size' = size - 1 : size' = size
    boolean removeFirstOccurrence(final Object query);
}
