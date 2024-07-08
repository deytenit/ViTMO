package info.kgeorgiy.ja.eremin.i18n;

import static info.kgeorgiy.ja.eremin.i18n.util.I18nUtilities.getBundle;

import info.kgeorgiy.ja.eremin.i18n.consumer.DateStatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.consumer.NumberStatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.consumer.StringStatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.localization.LocalizedStatisticsPipeline;
import info.kgeorgiy.ja.eremin.i18n.util.I18nUtilities;
import info.kgeorgiy.ja.eremin.util.ArgumentsResolver;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.text.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * Class for analyzing text and writing statistics to the file.
 *
 * @author Vladimir Eremin
 */
public class TextStatistics {
  private final ResourceBundle messageBundle;
  private final ResourceBundle statisticsBundle;

  private final LocalizedStatisticsPipeline<String, Double> sentences;
  private final LocalizedStatisticsPipeline<String, Double> words;
  private final LocalizedStatisticsPipeline<Number, Double> numbers;
  private final LocalizedStatisticsPipeline<Number, Double> currencies;
  private final LocalizedStatisticsPipeline<Date, Date> dates;

  /**
   * Creates a new instance of {@link TextStatistics}.
   *
   * @param textLocale the locale for the text
   * @param statsLocale the locale for the statistics
   */
  public TextStatistics(final Locale textLocale, final Locale statsLocale) {
    messageBundle = getBundle("messages", statsLocale);
    statisticsBundle = getBundle("statistics", statsLocale);

    sentences =
        new LocalizedStatisticsPipeline<>(
            "sentence",
            statisticsBundle,
            I18nUtilities::identityFormat,
            NumberFormat::getNumberInstance,
            new StringStatisticsConsumer(textLocale, BreakIterator::getSentenceInstance));
    words =
        new LocalizedStatisticsPipeline<>(
            "word",
            statisticsBundle,
            I18nUtilities::identityFormat,
            NumberFormat::getNumberInstance,
            new StringStatisticsConsumer(textLocale, BreakIterator::getWordInstance));
    numbers =
        new LocalizedStatisticsPipeline<>(
            "number",
            statisticsBundle,
            NumberFormat::getNumberInstance,
            NumberFormat::getNumberInstance,
            new NumberStatisticsConsumer(textLocale, NumberFormat::getNumberInstance));

    currencies =
        new LocalizedStatisticsPipeline<>(
            "currency",
            statisticsBundle,
            NumberFormat::getCurrencyInstance,
            NumberFormat::getCurrencyInstance,
            new NumberStatisticsConsumer(textLocale, NumberFormat::getCurrencyInstance));

    dates =
        new LocalizedStatisticsPipeline<>(
            "date",
            statisticsBundle,
            I18nUtilities::defaultDateFormat,
            I18nUtilities::defaultDateFormat,
            new DateStatisticsConsumer(textLocale));
  }

  /**
   * Entry point of the program.
   *
   * @param args the command-line arguments
   */
  public static void main(String[] args) {
    try {
      final ArgumentsResolver arguments = new ArgumentsResolver(args);

      final Locale textLocale = arguments.positional(0, Locale::forLanguageTag);
      final Locale statsLocale = arguments.positional(1, Locale::forLanguageTag);
      final Path textFilePath = arguments.positional(2, Path::of);
      final Path statsFilePath = arguments.positional(3, Path::of);

      final TextStatistics statistics = new TextStatistics(textLocale, statsLocale);

      statistics.analyze(textFilePath, statsFilePath);
    } catch (final ArgumentsResolver.ArgumentsResolverException e) {
      System.err.println(
          MessageFormat.format(
              I18nUtilities.FALLBACK_BUNDLE.getString("error_arguments"), e.getLocalizedMessage()));
      System.err.println(I18nUtilities.FALLBACK_BUNDLE.getString("usage"));
    } catch (final TextStatisticsException e) {
      System.err.println(e.getLocalizedMessage());
    }
  }

  /**
   * Analyzes the text from the file and writes the statistics to the file.
   *
   * <p>Writes the statistics to the file in the following format:
   * <pre>
   *     Analyzing file: "{textFilePath}".
   *     {overall}
   *     {groups.forEach(group -> {group.summary})}
   *
   * @param textFilePath the path to the file with the text
   * @param statsFilePath the path to the file to write the statistics
   * @throws TextStatisticsException if an error occurs during the analysis
   */
  public void analyze(final Path textFilePath, final Path statsFilePath)
      throws TextStatisticsException {
    try {
      final String text = Files.readString(textFilePath, StandardCharsets.UTF_8);
      try (final BufferedWriter writer = Files.newBufferedWriter(statsFilePath)) {
        writer.write(
            MessageFormat.format(
                    messageBundle.getString("analyzing_head"), textFilePath.getFileName())
                + "\n");
        statistics(text, writer);
      } catch (final IOException e) {
        throw new TextStatisticsException("error_write_stats", e);
      }
    } catch (final IOException e) {
      throw new TextStatisticsException("error_read_text", e);
    }
  }

  private void statistics(final String text, final Writer writer) throws IOException {
    Stream.of(sentences, words, numbers, currencies, dates)
        .parallel()
        .forEach(pipeline -> pipeline.consume(text));

    writer.write(LocalizedStatisticsPipeline.overallHead(statisticsBundle) + "\n");
    writer.write(
        Stream.of(sentences, words, numbers, currencies, dates)
                .map(LocalizedStatisticsPipeline::overall)
                .collect(Collectors.joining("\n"))
            + "\n");

    writer.write(fullSummary(sentences));
    writer.write(fullSummary(words));
    writer.write(shortSummary(numbers));
    writer.write(shortSummary(currencies));
    writer.write(shortSummary(dates));

    writer.flush();
  }

  private String fullSummary(final LocalizedStatisticsPipeline<?, ?> pipeline) {
    final String format =
        """
{0}
\t{1}
\t{2}
\t{3}
\t{4}
\t{5}
\t{6}
""";

    return MessageFormat.format(
        format,
        pipeline.contextHead(),
        pipeline.count(),
        pipeline.least(),
        pipeline.greatest(),
        pipeline.minimum(),
        pipeline.maximum(),
        pipeline.average());
  }

  private String shortSummary(final LocalizedStatisticsPipeline<?, ?> pipeline) {
    final String format =
        """
{0}
\t{1}
\t{2}
\t{3}
\t{4}
""";

    return MessageFormat.format(
        format,
        pipeline.contextHead(),
        pipeline.count(),
        pipeline.least(),
        pipeline.greatest(),
        pipeline.average());
  }

  /**
   * Exception for {@link TextStatistics}.
   *
   * <p>Wraps exceptions thrown by {@link TextStatistics}.
   *
   * <p>Provides {@link #getLocalizedMessage()} by key from {@link ResourceBundle}.
   */
  public class TextStatisticsException extends Exception {
    private final String key;

    /**
     * Constructs a new exception with the specified key.
     *
     * @param key the key for the localized message
     * @param cause the cause of the exception
     */
    public TextStatisticsException(final String key, final Throwable cause) {
      super(cause);
      this.key = key;
    }

    /**
     * Returns the localized message.
     *
     * @return the localized message
     */
    @Override
    public String getLocalizedMessage() {
      return MessageFormat.format(messageBundle.getString(key), super.getLocalizedMessage());
    }
  }
}
