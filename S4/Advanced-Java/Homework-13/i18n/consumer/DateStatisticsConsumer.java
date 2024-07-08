package info.kgeorgiy.ja.eremin.i18n.consumer;

import info.kgeorgiy.ja.eremin.i18n.supplier.DateStatisticsSupplier;
import java.text.BreakIterator;
import java.text.DateFormat;
import java.text.ParsePosition;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.stream.Stream;

/**
 * Class for consuming text and creating {@link DateStatisticsSupplier} from it.
 *
 * <p>Parses the following date formats:
 *
 * <ul>
 *   <li>{@link DateFormat#SHORT}
 *   <li>{@link DateFormat#DEFAULT}
 *   <li>{@link DateFormat#LONG}
 *   <li>{@link DateFormat#FULL}
 *
 * @see DateStatisticsSupplier
 * @author Vladimir Eremin
 */
public class DateStatisticsConsumer extends AbstractStatisticsConsumer<Date, Date> {
  private final List<DateFormat> formats;

  /**
   * Creates a new instance of {@link DateStatisticsConsumer}.
   *
   * @param locale {@link Locale} to use for parsing dates
   */
  public DateStatisticsConsumer(final Locale locale) {
    super(BreakIterator.getCharacterInstance(locale), DateStatisticsSupplier::new);
    formats =
        Stream.of(DateFormat.SHORT, DateFormat.DEFAULT, DateFormat.LONG, DateFormat.FULL)
            .map(style -> DateFormat.getDateInstance(style, locale))
            .toList();
  }

  @Override
  protected Date parse(final String text, final ParsePosition begin, final int end) {
    return formats.stream()
        .map(format -> format.parse(text, begin))
        .filter(Objects::nonNull)
        .findFirst()
        .orElse(null);
  }
}
