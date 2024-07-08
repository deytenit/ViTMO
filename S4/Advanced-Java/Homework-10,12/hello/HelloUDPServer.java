package info.kgeorgiy.ja.eremin.hello;

import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPResponder;
import info.kgeorgiy.java.advanced.hello.NewHelloServer;
import java.io.IOException;
import java.net.DatagramSocket;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * UDP implementation of {@link NewHelloServer}.
 *
 * <p>Listens to the specified ports and responds to the incoming packets.
 *
 * <p>Responds with a formatted body of request ({@link HelloUDPResponder}).
 *
 * @author Vladimir Eremin
 */
public class HelloUDPServer extends HelloUDPAbstractServer {
  private ExecutorService receivers;
  private ExecutorService responders;
  private List<DatagramSocket> sockets;

  /**
   * Entry point of the server.
   *
   * <p>Starts the server and listen to the specified port until user sends input.
   *
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    cli(args, HelloUDPServer::new);
  }

  @Override
  protected void receiver(final int port, final String format) throws IOException {
    final DatagramSocket socket = new DatagramSocket(port);
    sockets.add(socket);

    receivers.execute(
        () -> {
          final HelloUDPResponder responder = new HelloUDPResponder(socket, format);
          responder.accept(responders::execute);
        });
  }

  @Override
  protected void init(final int threads, final Map<Integer, String> ports) {
    receivers = Executors.newFixedThreadPool(ports.size());
    responders = Executors.newFixedThreadPool(threads);
    sockets = new ArrayList<>(ports.size());
  }

  /** Closes the server. */
  @Override
  public void close() {
    try {
      Objects.requireNonNull(responders).close();
      Objects.requireNonNull(sockets).forEach(DatagramSocket::close);
      Objects.requireNonNull(receivers).close();
    } catch (final NullPointerException ignored) {
    }
  }
}
