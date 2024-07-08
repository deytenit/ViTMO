package info.kgeorgiy.ja.eremin.i18n.test;

import info.kgeorgiy.ja.eremin.i18n.supplier.*;
import info.kgeorgiy.ja.eremin.i18n.util.AttributeStatistics;
import java.util.Date;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.MethodOrderer;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestMethodOrder;

/**
 * Test for {@link StatisticsSupplier}.
 *
 * <p>Just basic tests to see that there are no dumb mistakes in {@link StatisticsSupplier}. If it's
 * good, then {@link ConsumerStatisticsTest} is trustworthy.
 *
 * @author Vladimir Eremin
 * @see BaseStatisticsTest
 */
@DisplayName("Suppliers Units")
@TestMethodOrder(MethodOrderer.MethodName.class)
class SupplierStatisticsTest extends BaseStatisticsTest {
  @Test
  @DisplayName("Empty")
  public void test_1() {

    assertFull(
        new StringStatisticsSupplier(List.of()),
        Stream.of(0L, 0L, null, null, null, null, null).toList());
  }

  @Test
  @DisplayName("Numbers")
  public void test_2() {
    final List<Number> data = List.of(1228.4, 192168.0925, 0.500, 1228.4, -1244);

    assertDefault(
        new NumberStatisticsSupplier(data), List.of(5, 4, -1244, 192168.092, 38676.278399999996));
  }

  @Test
  @DisplayName("Dates")
  public void test_3() {
    final List<Date> data =
        List.of(
            new Date(0),
            new Date(1_228_800),
            new Date(1_290_044),
            new Date(1_228_800),
            new Date(1_000),
            new Date(1_337),
            new Date(1_800_555_35_35L));

    assertDefault(
        new DateStatisticsSupplier(data),
        List.of(7, 6, new Date(0), new Date(1_800_555_35_35L), new Date(2572757645L)));
  }

  @Test
  @DisplayName("Strings")
  public void test_4() {
    final List<String> data =
        List.of(
            "Hello, world!",
            "The BreakIterator class implements methods for finding the location of boundaries in text.",
            "Object.",
            "Modules (or maybe Quetza itself) can leverage external binaries that shall be located at bin/ directory or be accessible from the environment.",
            "Instant sharing utility web interface designed for programmatic or manual use.",
            "Object.",
            "Z",
            "Instant sharing utility web interface designed for programmatic or manual use.");

    assertFull(
        new StringStatisticsSupplier(data),
        List.of(
            8L,
            6L,
            data.get(0),
            data.get(6),
            new AttributeStatistics<>(data.get(6), 1.0),
            new AttributeStatistics<>(data.get(3), 142.0),
            52.0));
  }
}
