package info.kgeorgiy.ja.eremin.i18n.supplier;

import info.kgeorgiy.ja.eremin.i18n.util.AttributeStatistics;

/**
 * Interface for supplying statistics.
 *
 * <h3>Meaning behind tokens and attributes </h3>
 *
 * <p>Tokens are the units supplier actually collect statistics on. {@link #greatest()} and {@link
 * #least()} would return looking on the actual token of it.
 *
 * <p>Attribute is a token that is associated with a token. For example, such token can be a length
 * of token. Either way it should be a comparable token. {@link #maximum()} and {@link #minimum()}
 * would use it for their meaning.
 *
 * <p>Note that {@link #average()} returns just the attribute token, since its non-deterministic
 * what token to use.
 *
 * @param <T1> type of token
 * @param <T2> type of attribute
 * @see AttributeStatistics
 */
public interface StatisticsSupplier<T1, T2 extends Comparable<T2>> {
  /**
   * Returns the number of tokens.
   *
   * @return the number of tokens
   */
  long count();

  /**
   * Returns the number of unique tokens.
   *
   * @return the number of unique tokens
   */
  long unique();

  /**
   * Returns the greatest token by its token.
   *
   * @return the greatest token
   */
  T1 greatest();

  /**
   * Returns the least token by its token.
   *
   * @return the least token
   */
  T1 least();

  /**
   * Returns the token with the maximum attribute token.
   *
   * @return the token with the maximum attribute token
   */
  AttributeStatistics<T1, T2> maximum();

  /**
   * Returns the token with the minimum attribute token.
   *
   * @return the token with the minimum attribute token
   */
  AttributeStatistics<T1, T2> minimum();

  /**
   * Returns the average attribute token.
   *
   * @return the average attribute token
   */
  T2 average();
}
