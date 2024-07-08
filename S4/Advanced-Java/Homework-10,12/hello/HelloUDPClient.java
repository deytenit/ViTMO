package info.kgeorgiy.ja.eremin.hello;

import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPRequester;
import info.kgeorgiy.ja.eremin.hello.utils.HelloUDPUtilities;
import info.kgeorgiy.java.advanced.hello.HelloClient;
import java.io.IOException;
import java.net.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.stream.IntStream;

/**
 * UDP implementation of {@link HelloClient}.
 *
 * <p>Sends requests to the specified address and port.
 *
 * @author Vladimir Eremin
 * @see HelloClient
 */
public class HelloUDPClient extends HelloUDPAbstractClient {
  private SocketAddress address;
  private ExecutorService requesters;

  /**
   * Entry point of the client.
   *
   * <p>Starts the client and sends requests to the specified host.
   *
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    cli(args, HelloUDPClient::new);
  }

  @Override
  protected void init(String host, int port, int threads) throws IOException {
    address = new InetSocketAddress(InetAddress.getByName(host), port);
    requesters = Executors.newFixedThreadPool(threads);
  }

  @Override
  protected void requester(int threads, String prefix, int requests, String format) {
    IntStream.rangeClosed(1, threads)
        .forEach(
            thread ->
                requesters.execute(
                    () -> {
                      try (final DatagramSocket socket =
                          HelloUDPUtilities.createDatagramSocketWithTimeout()) {
                        final HelloUDPRequester requester =
                            new HelloUDPRequester(socket, address, format);

                        IntStream.rangeClosed(1, requests)
                            .forEach(id -> requester.accept(prefix + thread + "_" + id));
                      } catch (final SocketException e) {
                        System.err.println("Cannot open Datagram Socket. " + e.getMessage());
                      }
                    }));
  }

  @Override
  protected void close() {
    requesters.close();
  }
}
