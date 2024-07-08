package info.kgeorgiy.ja.eremin.hello;

import static info.kgeorgiy.ja.eremin.hello.utils.HelloUDPUtilities.GKStringFormat;

import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPNonblockingRequester;
import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPUtilities;
import info.kgeorgiy.java.advanced.hello.HelloClient;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.util.function.Function;
import java.util.stream.IntStream;

/**
 * Non-blocking UDP implementation of {@link HelloClient}.
 *
 * <p>Sends requests to the specified address and port.
 *
 * @author Vladimir Eremin
 * @see HelloClient
 */
public class HelloUDPNonblockingClient extends HelloUDPAbstractClient {
  private SocketAddress address;
  private Selector selector;

  /**
   * Entry point of the client.
   *
   * <p>Starts the client and sends requests to the specified host.
   *
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    cli(args, HelloUDPNonblockingClient::new);
  }

  @Override
  protected void init(String host, int port, int threads) throws IOException {
    address = new InetSocketAddress(InetAddress.getByName(host), port);
    selector = Selector.open();

    IntStream.rangeClosed(1, threads)
        .forEach(
            thread -> {
              try {
                final DatagramChannel channel = DatagramChannel.open();
                channel
                    .bind(null)
                    .configureBlocking(false)
                    .register(selector, SelectionKey.OP_WRITE)
                    .attach(new HelloUDPUtilities.ChannelContext(thread, 1));
              } catch (final IOException e) {
                System.err.println("Cannot open Datagram Channel. " + e.getMessage());
              }
            });
  }

  @Override
  protected void requester(int threads, String prefix, int requests, String format) {
    final Function<HelloUDPUtilities.ChannelContext, String> generator =
        ctx -> prefix + ctx.index() + "_" + ctx.requests();

    final HelloUDPNonblockingRequester requester =
        new HelloUDPNonblockingRequester(selector, address);
    requester.accept(
        generator,
        (response, ctx) -> {
          final String expected = GKStringFormat(format, generator.apply(ctx));
          if (response.equals(expected)) {
            ctx.inc();
          } else {
            System.err.format(
                "Received body is malformed.\nexpected: %s\nreceived: %s\n", expected, response);
          }

          return ctx.requests() > requests;
        });
  }

  @Override
  protected void close() {
    HelloUDPUtilities.closeIgnoring(selector);
  }
}
