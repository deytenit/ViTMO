package info.kgeorgiy.ja.eremin.i18n.test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertThrows;

import info.kgeorgiy.ja.eremin.i18n.*;
import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.List;
import org.junit.jupiter.api.*;

/**
 * Test for {@link TextStatistics}.
 *
 * <p>Basically tests for input preprocessing.
 *
 * @see TextStatistics
 * @author Vladimir Eremin
 */
@DisplayName("Text Statistics Integration")
@TestMethodOrder(MethodOrderer.MethodName.class)
class TextStatisticsTest extends BaseStatisticsTest {
  private final File output;
  private TextStatistics statistics;

  /**
   * Creates a new instance of {@link TextStatisticsTest}.
   *
   * @throws IOException if an I/O error occurs while creating temp output file
   */
  public TextStatisticsTest() throws IOException {
    output = File.createTempFile(this.getClass().getCanonicalName(), null);
    output.deleteOnExit();
  }

  @BeforeEach
  public void setUp() {
    statistics = new TextStatistics(ENGLISH, ENGLISH);
  }

  @Test
  @DisplayName("Empty file")
  public void test_40() {
    assertDoesNotThrow(() -> statistics.analyze(resourceToPath("empty"), output.toPath()));
  }

  @TestFactory
  @DisplayName("No File")
  public Iterable<DynamicTest> test_41() {
    return List.of(
        DynamicTest.dynamicTest(
            "Malformed text path",
            () ->
                assertThrows(
                    TextStatistics.TextStatisticsException.class,
                    () -> statistics.analyze(Path.of("aboboba"), output.toPath()))),
        DynamicTest.dynamicTest(
            "Malformed stats path",
            () ->
                assertThrows(
                    TextStatistics.TextStatisticsException.class,
                    () -> statistics.analyze(resourceToPath("empty"), Path.of("/aboba/aboba")))));
  }
}
