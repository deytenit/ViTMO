package info.kgeorgiy.ja.eremin.i18n.supplier;

import info.kgeorgiy.ja.eremin.i18n.util.AttributeStatistics;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;

/**
 * Abstract class for implementing {@link StatisticsSupplier}.
 *
 * @param <T1> type of token
 * @param <T2> type of attribute
 */
public abstract class AbstractStatisticsSupplier<T1, T2 extends Comparable<T2>>
    implements StatisticsSupplier<T1, T2> {
  protected final List<T1> data;

  protected AbstractStatisticsSupplier(final List<T1> data) {
    this.data = data;
  }

  protected abstract Comparator<T1> comparator();

  protected abstract AttributeStatistics<T1, T2> attribute(T1 value);

  protected abstract double order(T1 value);

  protected abstract T2 reorder(double value);

  /** {@inheritDoc} */
  @Override
  public long count() {
    return data.size();
  }

  /** {@inheritDoc} */
  @Override
  public long unique() {
    return new HashSet<>(data).size();
  }

  /** {@inheritDoc} */
  @Override
  public T1 greatest() {
    return data.stream().max(comparator()).orElse(null);
  }

  /** {@inheritDoc} */
  @Override
  public T1 least() {
    return data.stream().min(comparator()).orElse(null);
  }

  /** {@inheritDoc} */
  @Override
  public AttributeStatistics<T1, T2> maximum() {
    return data.stream().map(this::attribute).max(Comparator.naturalOrder()).orElse(null);
  }

  /** {@inheritDoc} */
  @Override
  public AttributeStatistics<T1, T2> minimum() {
    return data.stream().map(this::attribute).min(Comparator.naturalOrder()).orElse(null);
  }

  /** {@inheritDoc} */
  @Override
  public T2 average() {
    final var result = data.stream().mapToDouble(this::order).average();
    return result.isPresent() ? reorder(result.getAsDouble()) : null;
  }
}
