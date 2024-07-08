package info.kgeorgiy.ja.eremin.util;

import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * Tiny command line arguments retriever for specific tasks.
 *
 * @author Vladimir Eremin
 */
public class ArgumentsResolver {
  private static final String FLAG_PREFIX = "-";

  private final Set<String> flags;
  private final List<String> positional;

  /**
   * Constructs this {@link ArgumentsResolver}.
   *
   * <p>This does not perfome any parsing whatsoever, just checks whether provided arguments valid
   * for further fetching.
   *
   * @param args command line arguments.
   * @throws ArgumentsResolverException if provided arguments contain or is {@code null}.
   */
  public ArgumentsResolver(final String[] args) throws ArgumentsResolverException {
    if (args == null || Arrays.stream(args).anyMatch(Objects::isNull)) {
      throw new ArgumentsResolverException("Provided arguments are null.");
    }

    this.flags =
        Arrays.stream(args).filter(arg -> arg.startsWith(FLAG_PREFIX)).collect(Collectors.toSet());
    this.positional = Arrays.stream(args).filter(arg -> !arg.startsWith(FLAG_PREFIX)).toList();
  }

  /**
   * Checks whether provided flag had occurred in {@code args}.
   *
   * @param flag "-{flag name}" type of {@link String}
   * @return True if {@code flag} exists in {@link #flags}, False otherwise.
   */
  public boolean flag(final String flag) {
    return flags.contains(flag);
  }

  /**
   * Tries to retrieve and transform {@link #positional} argument.
   *
   * <p>{@code transformer} is a function that may parse or validate provided argument.
   *
   * @param position index of argument in {@link #positional} list.
   * @param transformer {@link Function} for transforming argument retrieved.
   * @param <T> desired return type.
   * @return result of {@code transformer} function.
   * @throws ArgumentsResolverException if {@code transformer} function failed.
   */
  public <T> T positional(final int position, final Function<String, T> transformer)
      throws ArgumentsResolverException {
    try {
      return transformer.apply(positional.get(position));
    } catch (final Exception e) {
      throw new ArgumentsResolverException(
          String.format(
              "Error while retrieving positional argument at position %d. %s",
              position, e.getMessage()));
    }
  }

  /**
   * Same as {@link #positional(int, Function)}, but returns fallback token if there is no argument
   * on such {@code position}.
   *
   * @param position index of argument in {@link #positional} list.
   * @param transformer {@link Function} for transforming argument retrieved.
   * @param orElse fallback token.
   * @param <T> desired return type.
   * @return result of {@code transformer} function.
   * @throws ArgumentsResolverException if {@code transformer} function failed.
   */
  public <T> T positional(final int position, final Function<String, T> transformer, final T orElse)
      throws ArgumentsResolverException {
    if (position < 0 || positional.size() <= position) {
      return orElse;
    }

    return positional(position, transformer);
  }

  /** Represents {@link Exception} occurred during work of {@link ArgumentsResolver}. */
  public static class ArgumentsResolverException extends Exception {
    /**
     * Constructs exception with given message.
     *
     * @param msg message explaining occurred problem.
     */
    public ArgumentsResolverException(final String msg) {
      super(msg);
    }
  }
}
