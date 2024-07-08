package info.kgeorgiy.ja.eremin.i18n.supplier;

import info.kgeorgiy.ja.eremin.i18n.util.AttributeStatistics;
import java.util.Comparator;
import java.util.Date;
import java.util.List;

public class DateStatisticsSupplier extends AbstractStatisticsSupplier<Date, Date> {
  /**
   * Creates a new instance of {@link DateStatisticsSupplier}.
   *
   * @param data list of dates as tokens
   */
  public DateStatisticsSupplier(final List<Date> data) {
    super(data);
  }

  @Override
  protected Comparator<Date> comparator() {
    return Comparator.naturalOrder();
  }

  @Override
  protected AttributeStatistics<Date, Date> attribute(Date value) {
    return new AttributeStatistics<>(value, value);
  }

  @Override
  protected double order(final Date value) {
    return value.getTime();
  }

  @Override
  protected Date reorder(final double value) {
    return new Date((long) value);
  }
}
