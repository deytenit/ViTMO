package info.kgeorgiy.ja.eremin.implementor;

import java.nio.file.Path;
import java.util.function.Predicate;

public class ImplementorKeywords {
  /** Current working directory for jvm process. */
  static final Path CWD = Path.of("");

  /** {@link Predicate} telling whether passed {@link Object} is a {@code '-jar'} {@link String}. */
  static final String JAR_FLAG = "-jar";

  /** Impl suffix for implementation class. */
  static final String IMPL = "Impl";

  /**
   * Width of the <i>tab</i>.
   *
   * @see ImplementorUtilities#tabulate(String, int)
   */
  static final int TABSTOP = 4;

  /** Writing: empty string. */
  static final String EMPTY = "";

  /** Writing: space symbol. */
  static final String SPACE = " ";

  /** Writing: comma space separator. */
  static final String SEP = ", ";

  /** Writing: new line. */
  static final String NEW_LINE = System.lineSeparator();

  /** Writing: double {@link #NEW_LINE}. */
  static final String NEW_BLOCK = NEW_LINE.repeat(2);

  /** Writing: semicolon. */
  static final String SEMICOLON = ";";

  /** Writing: open curly brace. */
  static final String OPEN_CURLY = "{";

  /** Writing: close curly brace. */
  static final String CLOSE_CURLY = "}";

  /** Writing: open parenthesis. */
  static final String OPEN_ROUNDY = "(";

  /** Writing: close parenthesis. */
  static final String CLOSE_ROUNDY = ")";

  /** Keywords: null (default non-primitive token) */
  static final String KW_NULL = "null";

  /** Keywords: false (default boolean token) */
  static final String KW_FALSE = "false";

  /** Keywords: 0 (default numerical token) */
  static final String KW_ZERO = "0";

  /** Keywords: class */
  static final String KW_CLASS = "class";

  /** Keywords: public */
  static final String KW_PUBLIC = "public";

  /** Keywords: implements */
  static final String KW_IMPLEMENTS = "implements";

  /** Keywords: package */
  static final String KW_PACKAGE = "package";

  /** Keywords: return */
  static final String KW_RETURN = "return";

  /** Keyword: throws */
  static final String KW_THROWS = "throws";

  /** Annotations: @Override */
  static final String AN_OVERRIDE = "@Override";
}
