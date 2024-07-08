package info.kgeorgiy.ja.eremin.i18n.test;

import info.kgeorgiy.ja.eremin.i18n.consumer.*;
import info.kgeorgiy.ja.eremin.i18n.supplier.DateStatisticsSupplier;
import info.kgeorgiy.ja.eremin.i18n.supplier.NumberStatisticsSupplier;
import java.io.IOException;
import java.text.DateFormat;
import java.text.NumberFormat;
import java.text.ParsePosition;
import java.util.*;
import java.util.function.DoubleFunction;
import java.util.function.IntFunction;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;
import org.junit.jupiter.api.*;

/**
 * Test for {@link StatisticsConsumer}.
 *
 * <p>Should only be trusted after {@link SupplierStatisticsTest} passed, as these tests are just
 * confirming data provided by the {@link StatisticsConsumer#consume(String)} method.
 *
 * @see BaseStatisticsTest
 */
@DisplayName("Consumers Units")
@TestMethodOrder(MethodOrderer.MethodName.class)
class ConsumerStatisticsTest extends BaseStatisticsTest {
  private final Random random = new Random(42);

  private static void numericalTest(
      NumberFormat instance, StatisticsConsumerTester<Number, Double> consumer) {
    final IntFunction<Number> transform = (x) -> x;

    final List<Number> integers = IntStream.range(0, 50).mapToObj(transform).toList();
    final List<Number> doubles = IntStream.range(0, 50).mapToObj(transform).toList();
    final List<Number> values = Stream.concat(integers.stream(), doubles.stream()).toList();

    final var text = values.stream().map(instance::format).collect(Collectors.joining("\n"));

    consumer.assertDefault(text, getDefaultStatistics(new NumberStatisticsSupplier(values)));
  }

  @TestFactory
  @DisplayName("Empty & Blank")
  public Iterable<DynamicTest> test_20() {
    setUp(ENGLISH);

    return List.of(
        DynamicTest.dynamicTest("Empty", () -> assertCounts("", List.of(0L, 0L, 0L, 0L, 0L))),
        DynamicTest.dynamicTest(
            "Blank", () -> assertCounts("              ", List.of(0L, 0L, 0L, 0L, 0L))),
        DynamicTest.dynamicTest(
            "Just Punctuation",
            () -> assertCounts(" !   ?   . . ! ..  --- ,   ", List.of(0L, 0L, 0L, 0L, 0L))));
  }

  @TestFactory
  @DisplayName("Random Number")
  public Stream<DynamicTest> test_21() {
    return localeTestFactory(
        (locale) -> {
          final NumberFormat instance = NumberFormat.getNumberInstance(locale);

          numericalTest(instance, number);
        });
  }

  @TestFactory
  @DisplayName("Random Date")
  public Stream<DynamicTest> test_22() {
    return localeTestFactory(
        (locale) -> {
          final DateFormat shortFormat = DateFormat.getDateInstance(DateFormat.SHORT, locale);
          final DateFormat defaultFormat = DateFormat.getDateInstance(DateFormat.DEFAULT, locale);
          final DateFormat longFormat = DateFormat.getDateInstance(DateFormat.LONG, locale);
          final DateFormat fullFormat = DateFormat.getDateInstance(DateFormat.FULL, locale);

          final List<Double> formatters =
              IntStream.range(0, 100).mapToObj(x -> random.nextDouble()).toList();

          final DoubleFunction<DateFormat> select =
              (x) -> {
                if (x < 0.25) return shortFormat;
                if (x < 0.5) return defaultFormat;
                if (x < 0.75) return longFormat;
                return fullFormat;
              };

          final List<Date> values =
              formatters.stream()
                  .map(
                      x -> {
                        final var format = select.apply(x);
                        return format.parse(
                            format.format(new Date(random.nextInt())), new ParsePosition(0));
                      })
                  .toList();

          final var text =
              IntStream.range(0, formatters.size())
                  .mapToObj(i -> select.apply(formatters.get(i)).format(values.get(i)))
                  .collect(Collectors.joining("\n"));

          date.assertDefault(text, getDefaultStatistics(new DateStatisticsSupplier(values)));
        });
  }

  @TestFactory
  @DisplayName("Random Currency")
  public Stream<DynamicTest> test_23() {
    return localeTestFactory(
        (locale) -> {
          final NumberFormat instance = NumberFormat.getCurrencyInstance(locale);

          numericalTest(instance, currency);
        });
  }

  @TestFactory
  @DisplayName("Simple Consumption")
  public Stream<DynamicTest> test_24() {
    final Map<Locale, List<Long>> actual =
        Map.of(
            ARABIC, List.of(25L, 336L, 82L, 1L, 0L),
            ENGLISH, List.of(26L, 306L, 82L, 10L, 17L),
            FRENCH, List.of(26L, 352L, 91L, 10L, 17L),
            JAPANESE, List.of(26L, 445L, 82L, 10L, 17L),
            RUSSIAN, List.of(21L, 268L, 82L, 10L, 17L));

    return localeTestFactory(
        (locale -> {
          try {
            assertFileCounts("simple", actual.get(locale));
          } catch (IOException e) {
            throw new AssertionError(e);
          }
        }));
  }

  @TestFactory
  @DisplayName("Long Consumption")
  public Stream<DynamicTest> test_25() {
    final Map<Locale, List<Long>> actual =
        Map.of(
            ARABIC, List.of(286L, 48865L, 1729L, 0L, 0L),
            ENGLISH, List.of(7524L, 115421L, 74L, 0L, 1L),
            FRENCH, List.of(1474L, 14121L, 18L, 0L, 0L),
            JAPANESE, List.of(374L, 13431L, 15L, 0L, 0L),
            RUSSIAN, List.of(19467L, 317304L, 134L, 0L, 0L));

    return localeTestFactory(
        (locale -> {
          try {
            assertFileCounts("long", actual.get(locale));
          } catch (IOException e) {
            throw new AssertionError(e);
          }
        }));
  }

  @TestFactory
  @DisplayName("Full-blown Consumption")
  public Stream<DynamicTest> test_26() {
      // NOTE: Wiki tests:
      // List of highest-grossing movies, even on different languages, uses usd currency, therefore
      // for locales other than en-US alternatives are used.
      // ARABIC: https://ar.wikipedia.org/wiki/%D8%A7%D9%84%D8%A5%D9%85%D8%A7%D8%B1%D8%A7%D8%AA_%D8%A7%D9%84%D8%B9%D8%B1%D8%A8%D9%8A%D8%A9_%D8%A7%D9%84%D9%85%D8%AA%D8%AD%D8%AF%D8%A9#%D8%A3%D9%87%D9%85_%D8%A5%D9%86%D8%AC%D8%A7%D8%B2%D8%A7%D8%AA_%D8%A7%D9%84%D9%82%D8%B1%D9%86_21
      // ENGLISH: https://en.wikipedia.org/wiki/List_of_highest-grossing_films
      // FRENCH: https://fr.wikipedia.org/wiki/Intouchables_(film)
      // JAPANESE: https://ja.wikipedia.org/wiki/%E6%97%A5%E6%9C%AC%E6%98%A0%E7%94%BB%E3%81%AE%E6%AD%B4%E4%BB%A3%E8%88%88%E8%A1%8C%E5%8F%8E%E5%85%A5%E4%B8%80%E8%A6%A7
      // RUSSIAN: https://ru.wikipedia.org/wiki/%D0%9C%D0%B0%D0%B9%D0%BE%D1%80_%D0%93%D1%80%D0%BE%D0%BC:_%D0%A7%D1%83%D0%BC%D0%BD%D0%BE%D0%B9_%D0%94%D0%BE%D0%BA%D1%82%D0%BE%D1%80
    final Map<Locale, List<Long>> actual =
        Map.of(
            ARABIC, List.of(429L, 15611L, 1506L, 4L, 111L),
            ENGLISH, List.of(137L, 10190L, 2651L, 967L, 2L),
            FRENCH, List.of(643L, 18695L, 4932L, 1L, 320L),
            JAPANESE, List.of(37L, 3028L, 758L, 30L, 3L),
            RUSSIAN, List.of(1574L, 23741L, 2778L, 2L, 174L));

    return localeTestFactory(
        locale -> {
          try {
            assertFileCounts("full-blown", actual.get(locale));
          } catch (IOException e) {
            throw new AssertionError(e);
          }
        });
  }
}
