package info.kgeorgiy.ja.eremin.hello.utils;

import info.kgeorgiy.ja.eremin.hello.HelloUDPServer;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import java.io.IOException;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedSelectorException;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.charset.StandardCharsets;
import java.util.Objects;

/**
 * Non-blocking responder for {@link HelloUDPServer}.
 *
 * <p>Responds to the incoming packets by sending them back with a formatted body.
 *
 * <p>Response format may contain a single marker that will be replaced with the actual data from
 * the received packet.
 *
 * <p>Response is sent back to the same address and port as the received packet.
 *
 * @author Vladimir Eremin
 * @see HelloUDPServer
 * @see SharedUtilities
 */
public class HelloUDPNonblockingResponder implements Runnable {
  private final Selector selector;

  /**
   * Creates a new instance of {@link HelloUDPNonblockingResponder}.
   *
   * @param selector {@link Selector} to select channels from.
   */
  public HelloUDPNonblockingResponder(final Selector selector) {
    this.selector = selector;
  }

  private void respond(final SelectionKey selectionKey) {
    final DatagramChannel channel = (DatagramChannel) selectionKey.channel();
    final ByteBuffer buffer;
    final SocketAddress address;
    try {
      buffer = ByteBuffer.allocate(HelloUDPUtilities.DEFAULT_BUFFER_CAPACITY);
      address = channel.receive(buffer);
      buffer.flip();
    } catch (final IOException e) {
      System.err.println("Error occurred while receiving a packet. " + e.getMessage());
      return;
    }

    final String data =
        HelloUDPUtilities.GKStringFormat(
            (String) selectionKey.attachment(), StandardCharsets.UTF_8.decode(buffer).toString());

    try {
      channel.send(
          ByteBuffer.wrap(data.getBytes(StandardCharsets.UTF_8)), Objects.requireNonNull(address));
    } catch (final IOException | NullPointerException e) {
      System.err.println("Error occurred while sending a packet. " + e.getMessage());
    }
  }

  /** Receives a packet, formats its body and sends it back. */
  @Override
  public void run() {
    while (selector.isOpen() && !Thread.interrupted()) {
      try {
        selector.select(
            selectionKey -> {
              if (selectionKey.isValid() && selectionKey.isReadable()) {
                respond(selectionKey);
              }
            },
            SharedUtilities.WATERFALL_TIMEOUT);
      } catch (final IOException | ClosedSelectorException e) {
        System.err.println("Error occurred while selecting key. " + e.getMessage());
      }
    }
  }
}
