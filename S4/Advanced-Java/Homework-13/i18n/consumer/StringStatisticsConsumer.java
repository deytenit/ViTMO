package info.kgeorgiy.ja.eremin.i18n.consumer;

import info.kgeorgiy.ja.eremin.i18n.supplier.StringStatisticsSupplier;
import java.text.BreakIterator;
import java.text.ParsePosition;
import java.util.Locale;
import java.util.function.Function;
import java.util.regex.Pattern;

/**
 * Class for consuming text and creating {@link StringStatisticsSupplier} from it.
 *
 * <p>Somewhat universal consumer with custom {@link BreakIterator}, but still bounded to {@link
 * Locale}.
 *
 * <p>Notice, this consumer only accepts {@link String Strings} containing anything but {@link
 * #PUNCTUATION}, that <a href="https://www.unicode.org/reports/tr18/#property_syntax">matches any
 * punctuation symbol</a>.
 *
 * @see StringStatisticsSupplier
 * @author Vladimir Eremin
 */
public class StringStatisticsConsumer extends AbstractStatisticsConsumer<String, Double> {
  private final Pattern PUNCTUATION =
      Pattern.compile("\\p{Punct}", Pattern.UNICODE_CHARACTER_CLASS);

  /**
   * Creates a new instance of {@link StringStatisticsConsumer}.
   *
   * @param locale {@link Locale} to use for parsing strings
   * @param iterator {@link Function} to create {@link BreakIterator} from {@link Locale}
   */
  public StringStatisticsConsumer(
      final Locale locale, final Function<Locale, BreakIterator> iterator) {
    super(iterator.apply(locale), StringStatisticsSupplier::new);
  }

  @Override
  protected String parse(final String text, final ParsePosition begin, final int end) {
    final var result = text.substring(begin.getIndex(), end).trim();
    if (!result.replaceAll(PUNCTUATION.pattern(), "").isBlank()) {
      begin.setIndex(end);
      return result;
    } else {
      return null;
    }
  }
}
