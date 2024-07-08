package info.kgeorgiy.ja.eremin.arrayset;

import java.util.*;

@SuppressWarnings("unused")
public class ArraySet<E> extends AbstractSet<E> implements SortedSet<E> {
  private final Comparator<? super E> comparator;
  private final List<E> elementData;

  public ArraySet() {
    this(List.of(), null);
  }

  public ArraySet(Comparator<? super E> comparator) {
    this(List.of(), comparator);
  }

  public ArraySet(Collection<? extends E> c) {
    this(c, null);
  }

  public ArraySet(Collection<? extends E> c, Comparator<? super E> comparator) {
    Set<E> tmp = new TreeSet<>(comparator);
    tmp.addAll(c);

    this.elementData = tmp.stream().toList();
    this.comparator = comparator;
  }

  /**
   * Constructs an array set containing the same elements and using the same ordering as the
   * specified sorted set.
   *
   * @param s sorted set whose elements will comprise the new set
   * @throws NullPointerException if the specified sorted set is null
   */
  public ArraySet(SortedSet<E> s) {
    this(s, s.comparator());
  }

  public static void main(String[] args) {
    SortedSet<Integer> set = new ArraySet<>(List.of(1, 10));

    var tmp = set.subSet(5, 3);

    for (final var i : tmp) {
      System.out.print(i);
    }

    System.out.println();
  }

  private ArraySet<E> makeEmptySet() {
    return new ArraySet<>(comparator);
  }

  @Override
  public Iterator<E> iterator() {
    return elementData.iterator();
  }

  @Override
  public int size() {
    return elementData.size();
  }

  @Override
  public boolean isEmpty() {
    return elementData.isEmpty();
  }

  @Override
  @SuppressWarnings("unchecked")
  public boolean contains(final Object o) {
    return getUnsafeIndex((E) o) >= 0;
  }

  private int getUnsafeIndex(final E e) {
    return Collections.binarySearch(elementData, e, comparator);
  }

  private int getIndex(final E e, boolean upper) {
    int idx = getUnsafeIndex(e);

    if (upper) {
      ++idx;
    }

    return idx >= 0 ? idx : -idx - 1;
  }

  @Override
  public Comparator<? super E> comparator() {
    return comparator;
  }

  @Override
  public SortedSet<E> subSet(final E fromElement, final E toElement) {
    if (comparator != null && comparator.compare(fromElement, toElement) > 0
        || Collections.reverseOrder(comparator).reversed().compare(fromElement, toElement) > 0) {
      throw new IllegalArgumentException(
          "Cannot create subset of negative span. FromElement > toElement.");
    }

    return makeSpan(fromElement, toElement, false);
  }

  @Override
  public SortedSet<E> headSet(final E toElement) {
    return !isEmpty() ? makeSpan(first(), toElement, false) : makeEmptySet();
  }

  @Override
  public SortedSet<E> tailSet(final E fromElement) {
    return !isEmpty() ? makeSpan(fromElement, last(), true) : makeEmptySet();
  }

  @Override
  public E first() {
    return elementData.getFirst();
  }

  @Override
  public E last() {
    return elementData.getLast();
  }

  private SortedSet<E> makeSpan(final E fromElement, final E toElement, boolean inclusive) {
    int from = getIndex(fromElement, false);
    int to = getIndex(toElement, inclusive);

    if (from > to) {
      return makeEmptySet();
    }

    return new ArraySet<>(elementData.subList(from, to), comparator);
  }
}
