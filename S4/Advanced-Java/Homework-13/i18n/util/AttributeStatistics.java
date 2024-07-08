package info.kgeorgiy.ja.eremin.i18n.util;

/**
 * Record for storing single attribute of a {@code token}.
 *
 * <p>This record compares with others by {@code attribute}.
 *
 * @param <T1> type of token
 * @param <T2> type of attribute
 * @see Comparable
 */
public record AttributeStatistics<T1, T2 extends Comparable<T2>>(T1 token, T2 attribute)
    implements Comparable<AttributeStatistics<T1, T2>> {

  /**
   * Compares this AttributeStatistics with the specified object over an {@code attribute} field.
   *
   * <p>{@inheritDoc}
   */
  @Override
  public int compareTo(AttributeStatistics<T1, T2> o) {
    return attribute.compareTo(o.attribute());
  }
}
