package info.kgeorgiy.ja.eremin.i18n.util;

import java.text.*;
import java.util.Locale;
import java.util.ResourceBundle;

/**
 * Utility class for i18n.
 *
 * @author Vladimir Eremin
 */
public class I18nUtilities {
  /**
   * Default bundle for messages.
   *
   * <p>Either way, even if such bundle is not found, a default one (with no locale) will be used
   * still.
   */
  public static final ResourceBundle FALLBACK_BUNDLE = getBundle("messages", Locale.getDefault());

  /**
   * Get bundle by name and locale.
   *
   * @param name bundle prefix name
   * @param locale {@link Locale} of a bundle
   * @return {@link ResourceBundle} instance
   */
  public static ResourceBundle getBundle(final String name, final Locale locale) {
    final String properties = "info.kgeorgiy.ja.eremin.i18n.localization.bundles." + name;
    return ResourceBundle.getBundle(properties, locale);
  }

  /**
   * Create identity format that works as {@link #toString()} of a provided {@link Object}.
   *
   * @param ignored {@link Locale} of a format (if it is even matter)
   * @return identity format
   */
  public static Format identityFormat(final Locale ignored) {
    return new Format() {
      @Override
      public StringBuffer format(Object obj, StringBuffer toAppendTo, FieldPosition pos) {
        return toAppendTo.append(obj.toString());
      }

      @Override
      public Object parseObject(String source, ParsePosition pos) {
        return source;
      }
    };
  }

  /**
   * Creates {@link DateFormat} with {@link DateFormat#DEFAULT} style.
   *
   * @param l {@link Locale} of a format
   * @return {@link DateFormat} instance
   */
  public static Format defaultDateFormat(final Locale l) {
    return DateFormat.getDateInstance(DateFormat.DEFAULT, l);
  }
}
