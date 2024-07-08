package info.kgeorgiy.ja.eremin.i18n.consumer;

import info.kgeorgiy.ja.eremin.i18n.supplier.StatisticsSupplier;
import java.text.BreakIterator;
import java.text.ParsePosition;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

/**
 * Abstract class for statistics consumer.
 *
 * <p>This class provides basic implementation for {@link StatisticsConsumer}.
 *
 * <p>It uses {@link BreakIterator} to split text into tokens and {@link #parse(String,
 * ParsePosition, int)} to create {@link StatisticsSupplier} from the result.
 *
 * @param <T1> type of token
 * @param <T2> type of attribute
 * @author Vladimir Eremin
 */
public abstract class AbstractStatisticsConsumer<T1, T2 extends Comparable<T2>>
    implements StatisticsConsumer<T1, T2> {
  protected final BreakIterator iterator;
  protected final Function<List<T1>, StatisticsSupplier<T1, T2>> supplier;

  /**
   * Creates a new instance of {@link AbstractStatisticsConsumer}.
   *
   * @param iterator {@link BreakIterator} to split text into tokens
   * @param supplier function to create {@link StatisticsSupplier} from the result
   */
  protected AbstractStatisticsConsumer(
      final BreakIterator iterator, Function<List<T1>, StatisticsSupplier<T1, T2>> supplier) {
    this.iterator = iterator;
    this.supplier = supplier;
  }

  protected abstract T1 parse(final String text, final ParsePosition begin, final int end);

  /** {@inheritDoc} */
  @Override
  public StatisticsSupplier<T1, T2> consume(String text) {
    iterator.setText(text);

    List<T1> result = new ArrayList<>();
    final ParsePosition begin = new ParsePosition(0);

    for (int beginIndex = iterator.first(), endIndex = iterator.next();
        endIndex != BreakIterator.DONE;
        beginIndex = endIndex, endIndex = iterator.next()) {
      begin.setIndex(beginIndex);

      final T1 token = parse(text, begin, endIndex);

      if (token != null) {
        result.add(token);
        while (endIndex != BreakIterator.DONE && endIndex < begin.getIndex()) {
          endIndex = iterator.next();
        }
      }
    }

    return supplier.apply(result);
  }
}
