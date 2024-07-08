package info.kgeorgiy.ja.eremin.i18n.test;

import static org.junit.jupiter.api.Assertions.assertEquals;

import info.kgeorgiy.ja.eremin.i18n.consumer.DateStatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.consumer.NumberStatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.consumer.StatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.consumer.StringStatisticsConsumer;
import info.kgeorgiy.ja.eremin.i18n.supplier.StatisticsSupplier;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.text.BreakIterator;
import java.text.NumberFormat;
import java.util.*;
import java.util.function.Consumer;
import java.util.stream.Stream;
import org.junit.jupiter.api.DynamicTest;
import org.junit.jupiter.api.extension.ExtendWith;
import org.junit.jupiter.api.extension.ExtensionContext;
import org.junit.jupiter.api.extension.TestWatcher;

/**
 * Base class for testing {@link StatisticsConsumer}.
 *
 * <p>Provides common methods for testing {@link StatisticsConsumer}.
 *
 * @see ConsumerStatisticsTest
 * @see SupplierStatisticsTest
 * @author Vladimir Eremin
 */
@ExtendWith(BaseStatisticsTest.TestStatusExtension.class)
public class BaseStatisticsTest {
  protected static final Locale RUSSIAN = Locale.of("ru", "RU");
  protected static final Locale ENGLISH = Locale.of("en", "US");
  protected static final Locale FRENCH = Locale.of("fr", "FR");
  protected static final Locale ARABIC = Locale.of("ar", "AE");
  protected static final Locale JAPANESE = Locale.of("ja", "JP");

  protected final ClassLoader loader = getClass().getClassLoader();
  protected Locale locale;

  protected StatisticsConsumerTester<String, Double> sentence;
  protected StatisticsConsumerTester<String, Double> word;
  protected StatisticsConsumerTester<Number, Double> number;
  protected StatisticsConsumerTester<Number, Double> currency;
  protected StatisticsConsumerTester<Date, Date> date;

  protected static List<?> getDefaultStatistics(final StatisticsSupplier<?, ?> supplier) {
    return Stream.of(
            supplier.count(),
            supplier.unique(),
            supplier.least(),
            supplier.greatest(),
            supplier.average())
        .toList();
  }

  protected static List<?> getFullStatistics(final StatisticsSupplier<?, ?> supplier) {
    return Stream.of(
            supplier.count(),
            supplier.unique(),
            supplier.least(),
            supplier.greatest(),
            supplier.minimum(),
            supplier.maximum(),
            supplier.average())
        .toList();
  }

  private static void assertLists(final List<?> expected, final List<?> actual) {
    final double delta = 1e-2;

    assertEquals(expected.size(), actual.size());
    for (int i = 0; i < expected.size(); i++) {
      final var e = expected.get(i);
      final var a = actual.get(i);

      if (e instanceof Number && a instanceof Number) {
        assertEquals(((Number) e).doubleValue(), ((Number) a).doubleValue(), delta);
      } else {
        assertEquals(expected.get(i), actual.get(i));
      }
    }
  }

  protected static void assertDefault(
      final StatisticsSupplier<?, ?> supplier, final List<?> expected) {
    final var actual = getDefaultStatistics(supplier);

    assertLists(expected, actual);
  }

  protected static void assertFull(
      final StatisticsSupplier<?, ?> supplier, final List<?> expected) {
    final var actual = getFullStatistics(supplier);

    assertLists(expected, actual);
  }

  protected Path resourceToPath(final String resource) {
    return Path.of(Objects.requireNonNull(loader.getResource(resource)).getPath());
  }

  private String getTextFile(final String prefix, final Locale locale) throws IOException {
    final Path textPath = resourceToPath(prefix + "_" + locale.getLanguage());
    return Files.readString(textPath, StandardCharsets.UTF_8);
  }

  protected void setUp(final Locale locale) {
    sentence =
        new StatisticsConsumerTester<>(
            new StringStatisticsConsumer(locale, BreakIterator::getSentenceInstance));
    word =
        new StatisticsConsumerTester<>(
            new StringStatisticsConsumer(locale, BreakIterator::getWordInstance));
    number =
        new StatisticsConsumerTester<>(
            new NumberStatisticsConsumer(locale, NumberFormat::getNumberInstance));
    currency =
        new StatisticsConsumerTester<>(
            new NumberStatisticsConsumer(locale, NumberFormat::getCurrencyInstance));
    date = new StatisticsConsumerTester<>(new DateStatisticsConsumer(locale));
    this.locale = locale;
  }

  protected Stream<DynamicTest> localeTestFactory(Consumer<Locale> test) {
    return Stream.of(ARABIC, ENGLISH, FRENCH, JAPANESE, RUSSIAN)
        .map(
            locale ->
                DynamicTest.dynamicTest(
                    locale.getLanguage(),
                    () -> {
                      setUp(locale);
                      test.accept(locale);
                    }));
  }

  protected void assertCounts(final String text, final List<Long> expected) {
    final var actual =
        Stream.of(sentence, word, number, currency, date)
            .map(consumer -> consumer.consume(text))
            .mapToLong(StatisticsSupplier::count);

    assertLists(expected, actual.boxed().toList());
  }

  protected void assertFileCounts(final String prefix, final List<Long> expected)
      throws IOException {
    final var text = getTextFile(prefix, locale);
    assertCounts(text, expected);
  }

  protected static class StatisticsConsumerTester<T1, T2 extends Comparable<T2>> {
    private final StatisticsConsumer<T1, T2> consumer;

    public StatisticsConsumerTester(final StatisticsConsumer<T1, T2> consumer) {
      this.consumer = consumer;
    }

    public StatisticsSupplier<T1, T2> consume(String text) {
      return consumer.consume(text);
    }

    public void assertDefault(final String text, final List<?> expected) {
      BaseStatisticsTest.assertDefault(consumer.consume(text), expected);
    }

    public void assertFull(final String text, final List<?> expected) {
      BaseStatisticsTest.assertFull(consumer.consume(text), expected);
    }
  }

  public static class TestStatusExtension implements TestWatcher {

    @Override
    public void testSuccessful(ExtensionContext context) {
      System.out.println("Test successful: " + context.getDisplayName());
    }

    @Override
    public void testFailed(ExtensionContext context, Throwable cause) {
      System.out.println("Test failed: " + context.getDisplayName());
      cause.printStackTrace();
    }

    @Override
    public void testAborted(ExtensionContext context, Throwable cause) {
      System.out.println("Test aborted: " + context.getDisplayName());
    }

    @Override
    public void testDisabled(ExtensionContext context, Optional<String> reason) {
      System.out.println("Test disabled: " + context.getDisplayName());
    }
  }
}
