package info.kgeorgiy.ja.eremin.implementor;

import static info.kgeorgiy.ja.eremin.implementor.ImplementorKeywords.*;

import info.kgeorgiy.ja.eremin.util.ArgumentsResolver;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import info.kgeorgiy.java.advanced.implementor.ImplerException;
import info.kgeorgiy.java.advanced.implementor.JarImpler;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.Writer;
import java.lang.reflect.AccessFlag;
import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.function.UnaryOperator;
import java.util.stream.Collectors;

/**
 * {@link JarImpler API} for implementing Java interfaces.
 *
 * <p>Leveraging {@link java.lang.reflect} API to implement a class for the interface specified.
 * Generated methods will return corresponding type {@link #generateDefaultValue(Class) default
 * token}.
 *
 * <h2><a id="impl-source">impl-source</a></h2>
 *
 * Is a java source code file that being generated by this class, more exact, by {@link
 * #implement(Class, Path)} method.
 *
 * <h2><a id="impl-class">impl-class</a></h2>
 *
 * Is a java class bytecode file that being generated by this class, more exact, it's <a
 * href="#impl-class">impl-class</a> that was {@link ImplementorUtilities#compile(Class, Path)
 * compiled}.
 *
 * @author Vladimir Eremin
 */
public class Implementor implements JarImpler {

  /**
   * Default constructor of {@link Implementor}.
   *
   * <p>Instance provide access to {@link JarImpler} methods.
   *
   * @see Implementor
   */
  public Implementor() {}

  /**
   * Generates default <i>return token</i> for {@link Class} (type) specified.
   *
   * <p>Default values:
   *
   * <ul>
   *   <li>{@code void}: {@link ImplementorKeywords#EMPTY}
   *   <li>{@code boolean}: {@link ImplementorKeywords#KW_FALSE false}
   *   <li>Non-primitives: {@link ImplementorKeywords#KW_NULL null}
   *   <li>Otherwise: {@link ImplementorKeywords#KW_ZERO 0}
   * </ul>
   *
   * <p>Returned string is eligible to be used as <i>return token</i>, see {@link
   * #generateMethodReturn(Method)}.
   *
   * @param type Class which default values should be generated.
   * @return {@link String} representing type's default token.
   * @see <a
   *     href="https://docs.oracle.com/javase/tutorial/java/nutsandbolts/datatypes.html">Primitive
   *     Data Types</a>
   */
  private static String generateDefaultValue(final Class<?> type) {
    if (type.equals(void.class)) {
      return EMPTY;
    }

    if (type.equals(boolean.class)) {
      return KW_FALSE;
    }

    if (!type.isPrimitive()) {
      return KW_NULL;
    }

    return KW_ZERO;
  }

  /**
   * Generates a <i>throws clause</i> for specified method.
   *
   * <p>Generated result is {@link ImplementorKeywords#EMPTY}, if Method doesn't <i>throw</i>
   * anything.
   *
   * <p>Returned string is eligible to be used as method's <i>throws clause</i>, see {@link
   * #generateMethodSignature(Method)}.
   *
   * @param method {@link Method} for witch generation is performed.
   * @return {@link String} representing method's <i>throws clause</i>.
   * @see <a
   *     href="https://docs.oracle.com/javase/tutorial/essential/exceptions/declaring.html">Throws
   *     Clause</a>
   */
  private static String generateMethodThrows(final Method method) {
    String result =
        Arrays.stream(method.getExceptionTypes())
            .map(Class::getCanonicalName)
            .collect(Collectors.joining(SEP));

    return !result.isEmpty() ? KW_THROWS + SPACE + result + SPACE : EMPTY;
  }

  /**
   * Generates the <i>arguments clause</i> for specified method.
   *
   * <p>Returned string is eligible to be used as method's <i>arguments clause</i>, see {@link
   * #generateMethodSignature(Method)}.
   *
   * @param method {@link Method} for witch generation is performed.
   * @return {@link String} representing method's <i>arguments clause</i>.
   * @see <a href="https://docs.oracle.com/javase/tutorial/java/javaOO/arguments.html">Passing
   *     Information to a Method</a>
   */
  private static String generateMethodParameters(final Method method) {
    return Arrays.stream(method.getParameters())
        .map(p -> p.getType().getCanonicalName() + SPACE + p.getName())
        .collect(Collectors.joining(SEP));
  }

  /**
   * Generates signature of the specified method.
   *
   * <p>Generates <i>public</i> {@link Override override} of an interface's method, i.e.
   *
   * <blockquote>
   *
   * <pre>
   * {@code @Override}
   * public (type) method({@link #generateMethodParameters(Method) arguments clause}) ({@link #generateMethodThrows(Method) throws clause}) {
   * </pre>
   *
   * </blockquote>
   *
   * <p>Method's <i>body</i> should be written right after this statement.
   *
   * @param method {@link Method} for witch generation is performed.
   * @return {@link String} representing method signature.
   * @see <a href="https://docs.oracle.com/javase/tutorial/java/javaOO/methods.html">Defining
   *     Methods</a>
   */
  private static String generateMethodSignature(final Method method) {
    return AN_OVERRIDE
        + NEW_LINE
        + KW_PUBLIC
        + SPACE
        + method.getReturnType().getCanonicalName()
        + SPACE
        + method.getName()
        + OPEN_ROUNDY
        + generateMethodParameters(method)
        + CLOSE_ROUNDY
        + SPACE
        + generateMethodThrows(method)
        + OPEN_CURLY
        + NEW_LINE;
  }

  /**
   * Generates <i>body</i> as return statement compiling with the <i>return type</i> of the
   * specified method.
   *
   * <p>Returned string is eligible to be used as method's <i>body</i>, see {@link
   * #writeMethods(Writer, Class)}.
   *
   * @param method {@link Method} for witch generation is performed.
   * @return {@link String} representing method return statement.
   * @see <a href="https://docs.oracle.com/javase/tutorial/java/javaOO/returnvalue.html">Returning a
   *     Value from a Method.</a>
   */
  private static String generateMethodReturn(final Method method) {
    final String value = generateDefaultValue(method.getReturnType());

    return KW_RETURN + (!value.isEmpty() ? SPACE : EMPTY) + value + SEMICOLON + NEW_LINE;
  }

  /**
   * Writes down methods implementations of the specified interface.
   *
   * <p>Each method defined as following:
   *
   * <blockquote>
   *
   * <pre>
   * ({@link #generateMethodSignature(Method) method signature})
   *  ({@link #generateMethodReturn(Method) method body})
   * }
   * </pre>
   *
   * </blockquote>
   *
   * <p>Resulting into well written java <i>class body</i>, see {@link #implement(Class, Path)}.
   *
   * @param writer {@link Writer} of currently generating <a href="#impl-source">impl-source</a>.
   * @param token {@link Class} for which generation is performed.
   * @throws IOException If an I/O error occurs.
   */
  private static void writeMethods(final Writer writer, final Class<?> token) throws IOException {
    final String generatedMethods =
        Arrays.stream(token.getMethods())
            .filter(
                method -> !method.isDefault() && !method.accessFlags().contains(AccessFlag.STATIC))
            .map(
                method ->
                    ImplementorUtilities.tabulate(generateMethodSignature(method), 1)
                        + ImplementorUtilities.tabulate(generateMethodReturn(method), 2)
                        + ImplementorUtilities.tabulate(CLOSE_CURLY, 1)
                        + NEW_BLOCK)
            .collect(Collectors.joining());

    writer.write(generatedMethods);
  }

  /**
   * Writes down the <i>package statement</i> of the specified interface implementation.
   *
   * <p>Uses the same package as specified interface. If specified interface <b>doesn't</b> belong
   * to any package does <b>nothing</b>.
   *
   * <p>Resulting into well written java <i>package statement</i>, see {@link #implement(Class,
   * Path)}.
   *
   * <p><b>Note:</b> this must be the first line in the generated <a
   * href="#impl-source">impl-source</a>.
   *
   * @param writer {@link Writer} of currently generating <a href="#impl-source">impl-source</a>.
   * @param token {@link Class} for which generation is performed.
   * @throws IOException If an I/O error occurs.
   * @see <a href="https://docs.oracle.com/javase/tutorial/java/package/createpkgs.html">Creating a
   *     Package</a>
   */
  private static void writePackageStatement(final Writer writer, final Class<?> token)
      throws IOException {
    final String packageName = token.getPackageName();

    if (packageName.isBlank()) {
      return;
    }

    writer.write(KW_PACKAGE + SPACE + packageName + SEMICOLON + NEW_BLOCK);
  }

  /**
   * Writes down the declaration of the implementation of specified interface.
   *
   * <p>Writes <i>public</i> Class that <i>implements</i> interface specified, i.e.
   *
   * <blockquote>
   *
   * <pre>
   * public class {@link ImplementorUtilities#getImplName(Class, boolean) interface-nameImpl} implements {@link Class#getCanonicalName() interface-name} {
   * </pre>
   *
   * </blockquote>
   *
   * <p>Resulting into well written java <i>class declaration</i>, see {@link #implement(Class,
   * Path)}.
   *
   * @param writer {@link Writer} of currently generating <a href="#impl-source">impl-source</a>.
   * @param token {@link Class} for which generation is performed.
   * @throws IOException If an I/O error occurs.
   * @see <a href="https://docs.oracle.com/javase/tutorial/java/javaOO/classdecl.html">Declaring
   *     Classes</a>
   */
  private static void writeClassDeclaration(final Writer writer, final Class<?> token)
      throws IOException {
    writer.write(
        KW_PUBLIC
            + SPACE
            + KW_CLASS
            + SPACE
            + SharedUtilities.unicodeEscape(ImplementorUtilities.getImplName(token, false))
            + SPACE
            + KW_IMPLEMENTS
            + SPACE
            + SharedUtilities.unicodeEscape(token.getCanonicalName())
            + SPACE
            + OPEN_CURLY
            + NEW_LINE);
  }

  /**
   * Entry point for the {@link Implementor} application.
   *
   * <p><b>In both modes</b> interface that shall be implemented is {@link
   * ImplementorUtilities#loadClass(String) resolved} relative to the {@link
   * ImplementorKeywords#CWD} (Current Working Directory).
   *
   * <h4>{@link #implementJar(Class, Path) Jar-mode}:</h4>
   *
   * <blockquote>
   *
   * <pre>
   * -jar (interface to implement) (output jar file)
   * </pre>
   *
   * </blockquote>
   *
   * Generates implementation for the specified (as first positional argument) interface and packs
   * it into Jar. Resulting Jar location is specified by the second positional argument.
   *
   * <h4>{@link #implement(Class, Path) Default}:</h4>
   *
   * <blockquote>
   *
   * <pre>
   * (interface to implement)
   * </pre>
   *
   * </blockquote>
   *
   * Generates implementation for the specified (as first positional argument) interface, and writes
   * it just near that specified interface.
   *
   * @param args {@link String}[] command line arguments.
   */
  public static void main(final String[] args) {
    try {
      final ArgumentsResolver arguments = new ArgumentsResolver(args);

      final Implementor implementor = new Implementor();

      if (arguments.flag(JAR_FLAG)) {
        try {
          final Class<?> token =
              ImplementorUtilities.loadClass(arguments.positional(0, UnaryOperator.identity()));
          final Path jarFile = Path.of(arguments.positional(1, UnaryOperator.identity()));
          implementor.implementJar(token, jarFile);
        } catch (final InvalidPathException err) {
          System.err.println("Invalid jar output file path was specified. " + err.getMessage());
        } catch (final ImplerException err) {
          System.err.println(err.getMessage());
        }
      } else {
        try {
          final Class<?> token =
              ImplementorUtilities.loadClass(arguments.positional(0, String::toString));
          implementor.implement(token, CWD);
        } catch (final InvalidPathException err) {
          throw new AssertionError(err);
        } catch (final ImplerException err) {
          System.err.println(err.getMessage());
        }
      }
    } catch (final ArgumentsResolver.ArgumentsResolverException e) {
      System.out.println(e.getMessage());
    }
  }

  /**
   * Implements the specified interface.
   *
   * <p>Creates the <a href="#impl-source">impl-source</a> just near the interface it shall
   * implement.
   *
   * <p>Created implementation is a well written java code, meaning it's compilable, i.e.
   *
   * <blockquote>
   *
   * <pre>
   * ({@link #writePackageStatement(Writer, Class) package statement})
   *
   * ({@link #writeClassDeclaration(Writer, Class) class declaration})
   *      ({@link #writeMethods(Writer, Class) class body})
   * }
   * </pre>
   *
   * </blockquote>
   *
   * <p>{@inheritDoc}
   *
   * @param token {@link Class} for which implementation is performed.
   * @param root {@link Path} is root relative to which specified interface is located.
   * @throws ImplerException If an error during implementation occurs.
   * @see <a
   *     href="https://docs.oracle.com/javase/tutorial/java/IandI/usinginterface.html">Implementing
   *     an Interface</a>
   */
  @Override
  public void implement(final Class<?> token, final Path root) throws ImplerException {
    if (token == null || !token.isInterface()) {
      throw new ImplerException("Provided token is not an Interface.");
    }

    if (token.accessFlags().contains(AccessFlag.PRIVATE)) {
      throw new ImplerException("Provided interface is private modified.");
    }

    if (token.getCanonicalName() == null) {
      throw new ImplerException("Cannot retrieve canonical name of the token.");
    }

    Path outputSourcePath;
    try {
      outputSourcePath = ImplementorUtilities.getImplPath(token, root, false);
    } catch (final InvalidPathException err) {
      throw new ImplerException("Cannot generate class file for impl-source." + err.getMessage());
    }

    try {
      SharedUtilities.createDirectories(outputSourcePath);
    } catch (final IOException err) {
      throw new ImplerException("Cannot create directories for impl-class." + err.getMessage());
    }

    try (BufferedWriter writer =
        Files.newBufferedWriter(outputSourcePath, StandardCharsets.UTF_8)) {
      try {
        writePackageStatement(writer, token);
        writeClassDeclaration(writer, token);
        writeMethods(writer, token);
        writer.write(CLOSE_CURLY + NEW_LINE);
      } catch (final IOException | SecurityException err) {
        throw new ImplerException("Error while writing to impl-source file. " + err.getMessage());
      }
    } catch (final IOException | SecurityException err) {
      throw new ImplerException(
          "Cannot create/access impl-source file to write. " + err.getMessage());
    }
  }

  /**
   * {@link #implement(Class, Path) Implements} the specified interface and {@link
   * ImplementorUtilities#makeJar(Class, Path, Path) packs} it into the Jar.
   *
   * <p>By {@link ImplementorUtilities#compile(Class, Path) compiling} <a
   * href="#impl-source">impl-source</a>, the resulted <a href="#impl-class">impl-class</a> is then
   * being packed and delivered as Jar to the desired destination.
   *
   * <p>In the process, generated files are being stored at a temp directory, that will be after
   * process completion deleted.
   *
   * <p>{@inheritDoc}
   *
   * @param token {@link Class} for which implementation is performed.
   * @param jarFile {@link Path} is the location of an output Jar.
   * @throws ImplerException If an error during Jar implementation occurs.
   * @see <a href="https://docs.oracle.com/javase/tutorial/deployment/jar/apiindex.html">Packaging
   *     Programs in JAR - Using JAR-related APIs</a>
   */
  @Override
  public void implementJar(final Class<?> token, final Path jarFile) throws ImplerException {
    Path jarFileParent = jarFile.toAbsolutePath().getParent().normalize();

    try {
      SharedUtilities.createDirectories(jarFileParent);
    } catch (final IOException err) {
      throw new ImplerException("Cannot create directories for impl-class." + err.getMessage());
    }

    Path tempDir;
    try {
      tempDir = Files.createTempDirectory(jarFileParent, null);
    } catch (final IOException | SecurityException err) {
      throw new ImplerException(
          "Cannot create temp directory for impl-source file. " + err.getMessage());
    }

    implement(token, tempDir);
    ImplementorUtilities.compile(token, tempDir);
    ImplementorUtilities.makeJar(token, tempDir, jarFile);

    SharedUtilities.deleteDirectory(tempDir);
  }
}