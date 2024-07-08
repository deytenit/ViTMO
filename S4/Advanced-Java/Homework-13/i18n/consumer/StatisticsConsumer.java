package info.kgeorgiy.ja.eremin.i18n.consumer;

import info.kgeorgiy.ja.eremin.i18n.supplier.StatisticsSupplier;

/**
 * Interface for consuming text and creating {@link StatisticsSupplier} from it.
 *
 * @param <T1> type of token
 * @param <T2> type of attribute
 * @author Vladimir Eremin
 * @see StatisticsSupplier
 */
public interface StatisticsConsumer<T1, T2 extends Comparable<T2>> {

  /**
   * Consumes text and creates {@link StatisticsSupplier} from it.
   *
   * @param text text to consume
   * @return {@link StatisticsSupplier} created from text
   */
  StatisticsSupplier<T1, T2> consume(final String text);
}
