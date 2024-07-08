package info.kgeorgiy.ja.eremin.hello.utils;

import info.kgeorgiy.ja.eremin.hello.HelloUDPServer;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.RejectedExecutionException;
import java.util.function.Consumer;

/**
 * Responder for {@link HelloUDPServer}.
 *
 * <p>Responds to the incoming packets by sending them back with a formatted body.
 *
 * <p>Response format is provided in {@link #format}.
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
public class HelloUDPResponder implements Consumer<Consumer<Runnable>> {
  private final DatagramSocket socket;
  private final String format;

  /**
   * Creates a new instance of {@link HelloUDPResponder}.
   *
   * @param socket {@link DatagramSocket} to communicate with.
   * @param format {@link String} to format response with.
   */
  public HelloUDPResponder(final DatagramSocket socket, final String format) {
    this.socket = socket;
    this.format = format;
  }

  /**
   * Receives a packet, formats its body and sends it back.
   *
   * @param responder to execute and respond result over the socket.
   */
  @Override
  public void accept(final Consumer<Runnable> responder) {
    while (!socket.isClosed() && !Thread.interrupted()) {
      DatagramPacket packet;
      try {
        packet = HelloUDPUtilities.makeReceivingDatagramPacket(socket.getReceiveBufferSize());
        socket.receive(packet);
      } catch (final IOException e) {
        System.err.println("Error occurred while receiving a packet. " + e.getMessage());
        continue;
      }

      try {
        responder.accept(
            () -> {
              packet.setData(
                  HelloUDPUtilities.GKStringFormat(
                          format, HelloUDPUtilities.getDatagramPacketDataUTF8(packet))
                      .getBytes(StandardCharsets.UTF_8));

              try {
                socket.send(packet);
              } catch (final IOException e) {
                System.err.println("Error occurred while sending a packet. " + e.getMessage());
              }
            });
      } catch (final RejectedExecutionException ignored) {
      }
    }
  }
}
