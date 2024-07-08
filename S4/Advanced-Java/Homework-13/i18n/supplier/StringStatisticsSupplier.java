package info.kgeorgiy.ja.eremin.i18n.supplier;

import info.kgeorgiy.ja.eremin.i18n.util.AttributeStatistics;
import java.util.Comparator;
import java.util.List;

public class StringStatisticsSupplier extends AbstractStatisticsSupplier<String, Double> {
  /**
   * Creates a new instance of {@link StringStatisticsSupplier}.
   *
   * @param data list of strings as tokens
   */
  public StringStatisticsSupplier(final List<String> data) {
    super(data);
  }

  @Override
  protected Comparator<String> comparator() {
    return Comparator.naturalOrder();
  }

  @Override
  protected AttributeStatistics<String, Double> attribute(String value) {
    return new AttributeStatistics<>(value, (double) value.length());
  }

  @Override
  protected double order(String value) {
    return value.length();
  }

  @Override
  protected Double reorder(double value) {
    return value;
  }
}
