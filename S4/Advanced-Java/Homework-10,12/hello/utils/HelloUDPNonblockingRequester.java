package info.kgeorgiy.ja.eremin.hello.utils;

import info.kgeorgiy.ja.eremin.hello.HelloUDPClient;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import java.io.IOException;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.charset.StandardCharsets;
import java.util.function.BiConsumer;
import java.util.function.BiPredicate;
import java.util.function.Function;

/**
 * Non-blocking requester for {@link HelloUDPClient}.
 *
 * <p>Sends requests to the specified address and port.
 *
 * <p>Request data is provided to {@link #accept}.
 *
 * @author Vladimir Eremin
 * @see HelloUDPClient
 * @see SharedUtilities
 */
public class HelloUDPNonblockingRequester
    implements BiConsumer<
        Function<HelloUDPUtilities.ChannelContext, String>,
        BiPredicate<String, HelloUDPUtilities.ChannelContext>> {
  private final Selector selector;
  private final SocketAddress address;

  /**
   * Creates a new instance of {@link HelloUDPNonblockingRequester}.
   *
   * @param selector {@link Selector} to select channels from.
   * @param address {@link SocketAddress} to send requests to.
   */
  public HelloUDPNonblockingRequester(final Selector selector, final SocketAddress address) {
    this.selector = selector;
    this.address = address;
  }

  private static void read(
      BiPredicate<String, HelloUDPUtilities.ChannelContext> confirm, SelectionKey selectionKey) {
    final DatagramChannel channel = (DatagramChannel) selectionKey.channel();
    final ByteBuffer buffer;
    try {
      buffer = ByteBuffer.allocate(HelloUDPUtilities.DEFAULT_BUFFER_CAPACITY);
      channel.receive(buffer);
      buffer.flip();

      selectionKey.interestOps(SelectionKey.OP_WRITE);

      if (confirm.test(
          StandardCharsets.UTF_8.decode(buffer).toString(),
          (HelloUDPUtilities.ChannelContext) selectionKey.attachment())) {
        selectionKey.cancel();
      }
    } catch (final IOException e) {
      System.err.println("Error occurred while receiving a packet. " + e.getMessage());
    }
  }

  /**
   * While {@link #selector} has keys, attempts to send the request and receive the response.
   *
   * @param generator {@link Function} to generate request body.
   * @param confirm {@link BiPredicate} to confirm the response.
   */
  @Override
  public void accept(
      final Function<HelloUDPUtilities.ChannelContext, String> generator,
      final BiPredicate<String, HelloUDPUtilities.ChannelContext> confirm) {
    while (!selector.keys().isEmpty() && !Thread.interrupted()) {
      try {
        final int selected =
            selector.select(
                selectionKey -> {
                  if (!selectionKey.isValid()) {
                    return;
                  }

                  if (selectionKey.isReadable()) {
                    read(confirm, selectionKey);
                  } else if (selectionKey.isWritable()) {
                    write(generator, selectionKey);
                  }
                },
                SharedUtilities.WATERFALL_TIMEOUT);

        if (selected == 0) {
          selector.keys().forEach(selectionKey -> selectionKey.interestOps(SelectionKey.OP_WRITE));
        }
      } catch (final IOException e) {
        System.err.println("Error occurred while selecting key. " + e.getMessage());
      }
    }
  }

  private void write(
      Function<HelloUDPUtilities.ChannelContext, String> generator, SelectionKey selectionKey) {
    final DatagramChannel channel = (DatagramChannel) selectionKey.channel();

    final ByteBuffer buffer =
        ByteBuffer.wrap(
            generator
                .apply((HelloUDPUtilities.ChannelContext) selectionKey.attachment())
                .getBytes(StandardCharsets.UTF_8));

    try {
      channel.send(buffer, address);
      selectionKey.interestOps(SelectionKey.OP_READ);
    } catch (final IOException e) {
      System.err.println("Error occurred while sending a packet. " + e.getMessage());
    }
  }
}
