package info.kgeorgiy.ja.eremin.hello;

import static info.kgeorgiy.ja.eremin.hello.utils.HelloUDPUtilities.DEFAULT_FORMAT;

import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPUtilities;
import info.kgeorgiy.ja.eremin.util.ArgumentsResolver;
import info.kgeorgiy.java.advanced.hello.HelloClient;
import java.io.IOException;
import java.util.function.Supplier;
import java.util.function.UnaryOperator;

public abstract class HelloUDPAbstractClient implements HelloClient {
  private static final int DEFAULT_THREADS = 12;
  private static final int DEFAULT_REQUEST = 12;

  protected static void cli(final String[] args, final Supplier<HelloClient> client) {
    try {
      final ArgumentsResolver arguments = new ArgumentsResolver(args);

      client
          .get()
          .run(
              arguments.positional(0, UnaryOperator.identity()),
              arguments.positional(1, Integer::parseInt),
              arguments.positional(2, UnaryOperator.identity(), HelloUDPUtilities.DEFAULT_PREFIX),
              arguments.positional(3, Integer::parseInt, DEFAULT_THREADS),
              arguments.positional(3, Integer::parseInt, DEFAULT_REQUEST));
    } catch (final ArgumentsResolver.ArgumentsResolverException e) {
      System.out.println(e.getMessage());
      System.out.println("Usage: HelloUDPClient host port [prefix [threads [requests]]]");
    }
  }

  /**
   * Sends requests to the specified address and port, with {@link HelloUDPUtilities#DEFAULT_FORMAT}
   * of response message.
   *
   * @param host server's name or IP address.
   * @param port server's port.
   * @param prefix prefix of request body to send.
   * @param threads number of working threads.
   * @param requests number of requests to send from each thread.
   * @see #run(String, int, String, int, int, String)
   */
  @Override
  public void run(String host, int port, String prefix, int threads, int requests) {
    run(host, port, prefix, threads, requests, DEFAULT_FORMAT);
  }

  /**
   * Sends requests to the specified address and port.
   *
   * <p>Request's body is a string formatted as follows: {@code prefix + thread-number + "_" +
   * request-number}
   *
   * <p>Runs until all requests are sent.
   *
   * @param host server's name or IP address.
   * @param port server's port.
   * @param prefix prefix of request body to send.
   * @param threads number of working threads.
   * @param requests number of requests to send from each thread.
   * @param format expected format of response.
   */
  public void run(
      String host, int port, String prefix, int threads, int requests, final String format) {
    try {
      init(host, port, threads);
    } catch (final IOException e) {
      System.err.println("Cannot create client. " + e.getMessage());
      return;
    }

    requester(threads, prefix, requests, format);

    close();
  }

  protected abstract void init(final String host, final int port, final int threads)
      throws IOException;

  protected abstract void requester(
      final int threads, final String prefix, int requests, final String format);

  protected abstract void close();
}
