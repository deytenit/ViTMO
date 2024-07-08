package info.kgeorgiy.ja.eremin.util;

import java.io.File;
import java.io.IOException;
import java.net.DatagramSocket;
import java.net.MalformedURLException;
import java.net.URI;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.stream.Stream;

/**
 * Provides various shared utilities whether functions of classes for actual solutions.
 *
 * @author Vladimir Eremin
 */
public class SharedUtilities {
  /**
   * Timeout for the {@link DatagramSocket} waterfalls.
   *
   * @see DatagramSocket#setSoTimeout(int)
   */
  public static final int WATERFALL_TIMEOUT = 228;

  /**
   * Checks whether the provided {@link String} is valid <a
   * href="http://tools.ietf.org/html/rfc3986">URL</a>,
   *
   * <p>This method works as {@link URI#create(String)}, but if it was of {@link java.net.URL}; any
   * {@link MalformedURLException} thrown by the {@link URI#toURL()} method is caught and wrapped in
   * a new {@link IllegalArgumentException} object, which is then thrown.
   *
   * @param url String to check for being valid URL.
   * @return the same string if URL is valid.
   * @throws IllegalArgumentException if provided String is not a valid URL.
   */
  public static String isValidURL(final String url) {
    try {
      return URI.create(url).toURL().toString();
    } catch (final MalformedURLException e) {
      throw new IllegalArgumentException(e);
    }
  }

  /**
   * Converts unicode string to unicode escaped string.
   *
   * <p>For each character in provided string, either ads it with no change if basic ASCII, or
   * converts it to {@code \\uxxxx} (excluding escape slashes) format if not.
   *
   * <p>Example: {@code šđčćasdf} -> {@code \\u0161\\u0111\\u010d\\u0107asdf} (excluding escape
   * slashes) (may not be visible at render, but it's escaped)
   *
   * @param str {@link String} that is unicode escaped.
   * @return String with unicode characters escaped.
   * @see <a href="https://docs.oracle.com/javase/tutorial/i18n/text/unicode.html">Unicode in
   *     Java</a>
   * @see <a href="https://en.wikipedia.org/wiki/ASCII">ASCII</a>
   */
  public static String unicodeEscape(final String str) {
    final StringBuilder out = new StringBuilder();
    for (int i = 0; i < str.length(); i++) {
      final char ch = str.charAt(i);
      if (ch <= 127) {
        out.append(ch);
      } else out.append("\\u").append(String.format("%04x", (int) ch));
    }

    return out.toString();
  }

  /**
   * Creates directories.
   *
   * <p>Utility function that silently deletes directory. Meaning that regardless of the success,
   * produces nothing whatsoever.
   *
   * @param dir {@link Path} of the directory to delete.
   * @throws IOException if an I/O error occurs.
   * @see File#delete()
   */
  public static void createDirectories(final Path dir) throws IOException {
    if (dir.normalize().getParent() != null) {
      Files.createDirectories(dir.normalize().getParent());
    }
  }

  /**
   * Deletes directory.
   *
   * <p>Utility function that silently deletes directory. Meaning that regardless of the success,
   * produces nothing whatsoever.
   *
   * @param dir {@link Path} of the directory to delete.
   * @see File#delete()
   */
  @SuppressWarnings("ResultOfMethodCallIgnored")
  public static void deleteDirectory(final Path dir) {
    try (Stream<Path> walker = Files.walk(dir)) {
      walker.sorted(Comparator.reverseOrder()).map(Path::toFile).forEach(File::delete);
    } catch (final IOException ignored) {
    }
  }

  /**
   * Awaits for user to press Enter key (reads lines), while printing messages stating the following
   * end of a program.
   *
   * <p>Call it at the end of the main method to make the program wait for user input.
   */
  @SuppressWarnings("ResultOfMethodCallIgnored")
  public static void pressAnyKeyToContinue() {
    System.out.println("Press Enter key to exit...");
    try {
      System.in.read();
    } catch (final IOException ignored) {
    }
    System.out.println("Exiting...");
  }
}
