package info.kgeorgiy.ja.eremin.hello;

import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPResponder;
import info.kgeorgiy.ja.eremin.util.ArgumentsResolver;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import info.kgeorgiy.java.advanced.hello.NewHelloServer;
import java.io.IOException;
import java.util.Map;
import java.util.function.Supplier;

/**
 * UDP implementation of {@link NewHelloServer}.
 *
 * <p>Listens to the specified ports and responds to the incoming packets.
 *
 * <p>Responds with a formatted body of request ({@link HelloUDPResponder}).
 *
 * @author Vladimir Eremin
 */
public abstract class HelloUDPAbstractServer implements NewHelloServer {
  private static final int DEFAULT_THREADS = 12;

  protected static void cli(String[] args, final Supplier<NewHelloServer> serverGen) {
    try (var server = serverGen.get()) {
      final ArgumentsResolver arguments = new ArgumentsResolver(args);

      server.start(
          arguments.positional(0, Integer::parseInt),
          arguments.positional(1, Integer::parseInt, DEFAULT_THREADS));

      SharedUtilities.pressAnyKeyToContinue();
    } catch (final ArgumentsResolver.ArgumentsResolverException e) {
      System.out.println(e.getMessage());
      System.out.println("Usage: HelloUDPServer port [threads]");
    }
  }

  /**
   * Starts the server and listens to the specified ports.
   *
   * <p>Creates a new {@link HelloUDPResponder} for each port and starts the specified number of
   * threads to listen to them.
   *
   * <p>Shall return instantly after executing.
   *
   * @param threads number of working threads.
   * @param ports ports to listen to and their format strings respectfully.
   */
  @Override
  public void start(final int threads, final Map<Integer, String> ports) {
    if (ports.isEmpty()) {
      return;
    }

    try {
      init(threads, ports);
    } catch (final IOException e) {
      System.err.println("Cannot open Server. " + e.getMessage());
      return;
    }

    ports.forEach(
        (port, format) -> {
          try {
            receiver(port, format);
          } catch (final IOException e) {
            System.err.println("Cannot open Datagram Socket. " + e.getMessage());
          }
        });
  }

  protected abstract void receiver(final int port, final String format) throws IOException;

  protected abstract void init(final int threads, final Map<Integer, String> ports)
      throws IOException;
}
