package info.kgeorgiy.ja.eremin.hello;

import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPNonblockingResponder;
import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPResponder;
import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPUtilities;
import info.kgeorgiy.java.advanced.hello.NewHelloServer;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Non-blocking UDP implementation of {@link NewHelloServer}.
 *
 * <p>Listens to the specified ports and responds to the incoming packets.
 *
 * <p>Responds with a formatted body of request ({@link HelloUDPResponder}).
 *
 * @author Vladimir Eremin
 */
public class HelloUDPNonblockingServer extends HelloUDPAbstractServer {
  private Selector selector;
  private ExecutorService responders;
  private List<DatagramChannel> channels;

  /**
   * Entry point of the server.
   *
   * <p>Starts the server and listen to the specified port until user sends input.
   *
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    cli(args, HelloUDPNonblockingServer::new);
  }

  @Override
  protected void receiver(int port, final String format) throws IOException {
    final DatagramChannel channel = DatagramChannel.open();
    channel
        .bind(new InetSocketAddress(port))
        .configureBlocking(false)
        .register(selector, SelectionKey.OP_READ)
        .attach(format);
    channels.add(channel);

    responders.execute(
        () -> {
          final HelloUDPNonblockingResponder responder = new HelloUDPNonblockingResponder(selector);
          responder.run();
        });
  }

  @Override
  protected void init(int threads, Map<Integer, String> ports) throws IOException {
    selector = Selector.open();
    responders = Executors.newSingleThreadExecutor();
    channels = new ArrayList<>(ports.size());
  }

  /** Closes the server. */
  @Override
  public void close() {
    try {
      Objects.requireNonNull(channels).forEach(HelloUDPUtilities::closeIgnoring);
      HelloUDPUtilities.closeIgnoring(selector);
      Objects.requireNonNull(responders).close();
    } catch (final NullPointerException ignored) {
    }
  }
}
