package info.kgeorgiy.ja.eremin.i18n.localization;

import info.kgeorgiy.ja.eremin.i18n.consumer.StatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.supplier.StatisticsSupplier;
import info.kgeorgiy.ja.eremin.i18n.util.AttributeStatistics;
import java.text.Format;
import java.text.MessageFormat;
import java.text.NumberFormat;
import java.util.Locale;
import java.util.ResourceBundle;
import java.util.function.Function;

/**
 * Class for localization of {@link StatisticsConsumer} that provides {@link StatisticsSupplier}
 * after {@link #consume(String)}, so it is a pipeline.
 *
 * @param <T1> type of token
 * @param <T2> type of attribute
 */
public class LocalizedStatisticsPipeline<T1, T2 extends Comparable<T2>> {
  private final String prefix;
  private final ResourceBundle bundle;
  private final Format countFormat;
  private final Format tokenFormat;
  private final Format attributeFormat;
  private final StatisticsConsumer<T1, T2> consumer;
  private StatisticsSupplier<T1, T2> supplier;

  /**
   * Creates a new instance of {@link LocalizedStatisticsPipeline}.
   *
   * @param prefix prefix of the bundle
   * @param bundle {@link ResourceBundle} to use for localization
   * @param tokenFormat {@link Function} to create {@link Format} for tokens
   * @param attributeFormat {@link Function} to create {@link Format} for attributes
   * @param consumer {@link StatisticsConsumer} to use for collecting statistics
   */
  public LocalizedStatisticsPipeline(
      final String prefix,
      final ResourceBundle bundle,
      final Function<Locale, Format> tokenFormat,
      final Function<Locale, Format> attributeFormat,
      final StatisticsConsumer<T1, T2> consumer) {
    this.prefix = prefix;
    this.bundle = bundle;
    this.countFormat = NumberFormat.getNumberInstance(bundle.getLocale());
    this.tokenFormat = tokenFormat.apply(bundle.getLocale());
    this.attributeFormat = attributeFormat.apply(bundle.getLocale());
    this.consumer = consumer;
  }

  /**
   * Get head string for overall statistics.
   *
   * @param bundle {@link ResourceBundle} to use for localization
   * @return localized overall head
   */
  public static String overallHead(final ResourceBundle bundle) {
    return bundle.getString("overall_head");
  }

  /**
   * Consumes text and creates {@link StatisticsSupplier} from it.
   *
   * @param text text to consume
   */
  public void consume(final String text) {
    supplier = consumer.consume(text);
  }

  /**
   * Get localized string for undefined value.
   *
   * @return localized undefined statement
   */
  public String undefined() {
    return bundle.getString("undefined");
  }

  /**
   * Formats value if it is not null, otherwise returns{@link #undefined()}.
   *
   * @return formatted value or result of {@link #undefined()}
   */
  private <T> String formatElseUndefined(final Function<T, String> format, final T value) {
    return value == null ? undefined() : format.apply(value);
  }

  /**
   * Get overall statistics.
   *
   * <p>Provided by: {@link StatisticsSupplier#count()}.
   *
   * @return localized overall statistics
   */
  public String overall() {
    return MessageFormat.format(
        bundle.getString("overall_" + prefix), countFormat.format(supplier.count()));
  }

  /**
   * Get context head string for this group of statistics, defined by {@link #prefix}.
   *
   * @return localized context head
   */
  public String contextHead() {
    return bundle.getString(prefix + "_head");
  }

  /**
   * Get count statistics.
   *
   * <p>Provided by: {@link StatisticsSupplier#count()} and {@link StatisticsSupplier#unique()}.
   *
   * @return localized count statistics
   */
  public String count() {
    return MessageFormat.format(
        bundle.getString(prefix + "_count"),
        countFormat.format(supplier.count()),
        numerical("unique", (int) supplier.unique()));
  }

  /**
   * Get the least value of statistics.
   *
   * <p>Provided by: {@link StatisticsSupplier#least()}.
   *
   * @return localized least value
   */
  public String least() {
    return formatValue("least", supplier.least());
  }

  /**
   * Get the greatest value of statistics.
   *
   * <p>Provided by: {@link StatisticsSupplier#greatest()}.
   *
   * @return localized greatest value
   */
  public String greatest() {
    return formatValue("greatest", supplier.greatest());
  }

  /**
   * Get the minimum value of statistics.
   *
   * <p>Provided by: {@link StatisticsSupplier#minimum()}.
   *
   * @return localized minimum value
   */
  public String minimum() {
    return formatAttributeAndValue("minimum", supplier.minimum());
  }

  /**
   * Get the maximum value of statistics.
   *
   * <p>Provided by: {@link StatisticsSupplier#maximum()}.
   *
   * @return localized maximum value
   */
  public String maximum() {
    return formatAttributeAndValue("maximum", supplier.maximum());
  }

  /**
   * Get the average value of statistics.
   *
   * <p>Provided by: {@link StatisticsSupplier#average()}.
   *
   * @return localized average value
   */
  public String average() {
    return formatAttribute("average", supplier.average());
  }

  private <T> String formatValue(final String suffix, final T value) {
    return MessageFormat.format(
        bundle.getString(prefix + "_" + suffix), formatElseUndefined(tokenFormat::format, value));
  }

  private <T> String formatAttribute(final String suffix, final T value) {
    return MessageFormat.format(
        bundle.getString(prefix + "_" + suffix),
        formatElseUndefined(attributeFormat::format, value));
  }

  private String formatAttributeAndValue(
      final String suffix, final AttributeStatistics<T1, T2> value) {

    if (value == null) {
      return undefined();
    }

    return MessageFormat.format(
        bundle.getString(prefix + "_" + suffix),
        formatElseUndefined(attributeFormat::format, value.attribute()),
        formatElseUndefined(tokenFormat::format, value.token()));
  }

  private String numerical(final String prefix, final int value) {
    final String key =
        switch (value % 10) {
          case 0 -> "none";
          case 1 -> "single";
          default -> "multiple";
        };

    return MessageFormat.format(bundle.getString(prefix + "_" + key), countFormat.format(value));
  }
}
