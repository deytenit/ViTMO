package info.kgeorgiy.ja.eremin.i18n.consumer;

import info.kgeorgiy.ja.eremin.i18n.supplier.NumberStatisticsSupplier;
import info.kgeorgiy.ja.eremin.i18n.supplier.StatisticsSupplier;
import java.text.BreakIterator;
import java.text.NumberFormat;
import java.text.ParsePosition;
import java.util.Locale;
import java.util.function.Function;

/**
 * Class for consuming text and creating {@link NumberStatisticsSupplier} from it.
 *
 * @see NumberStatisticsSupplier
 * @author Vladimir Eremin
 */
public class NumberStatisticsConsumer extends AbstractStatisticsConsumer<Number, Double> {
  private final NumberFormat format;
  private final String currencyUnifierPattern;

  /**
   * Creates a new instance of {@link NumberStatisticsConsumer}.
   *
   * @param locale {@link Locale} to use for parsing numbers
   * @param formatter {@link Function} to create {@link NumberFormat} from {@link Locale}
   */
  public NumberStatisticsConsumer(
      final Locale locale, final Function<Locale, NumberFormat> formatter) {
    super(BreakIterator.getCharacterInstance(locale), NumberStatisticsSupplier::new);

    this.format = formatter.apply(locale);

    final String symbol = format.getCurrency().getSymbol(locale);
    this.currencyUnifierPattern = "(?<=[\\d%s])[ ]+(?=[\\d%s])".formatted(
            symbol, symbol
    );
  }

  @Override
  public StatisticsSupplier<Number, Double> consume(String text) {
    return super.consume(text.replaceAll(currencyUnifierPattern,
            " "));
  }

  @Override
  protected Number parse(final String text, final ParsePosition begin, final int end) {
    return format.parse(
        text,
        begin);
  }
}
