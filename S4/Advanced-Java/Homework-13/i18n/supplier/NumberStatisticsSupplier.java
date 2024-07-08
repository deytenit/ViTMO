package info.kgeorgiy.ja.eremin.i18n.supplier;

import info.kgeorgiy.ja.eremin.i18n.util.AttributeStatistics;
import java.util.Comparator;
import java.util.List;

public class NumberStatisticsSupplier extends AbstractStatisticsSupplier<Number, Double> {
  /**
   * Creates a new instance of {@link NumberStatisticsSupplier}.
   *
   * @param data list of numbers as tokens
   */
  public NumberStatisticsSupplier(final List<Number> data) {
    super(data);
  }

  @Override
  protected Comparator<Number> comparator() {
    return Comparator.comparingDouble(Number::doubleValue);
  }

  @Override
  protected AttributeStatistics<Number, Double> attribute(Number value) {
    return new AttributeStatistics<>(value, value.doubleValue());
  }

  @Override
  protected double order(Number value) {
    return value.doubleValue();
  }

  @Override
  protected Double reorder(double value) {
    return value;
  }
}
