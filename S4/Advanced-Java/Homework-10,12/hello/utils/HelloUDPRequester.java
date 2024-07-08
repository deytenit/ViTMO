package info.kgeorgiy.ja.eremin.hello.utils;

import info.kgeorgiy.ja.eremin.hello.HelloUDPClient;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketAddress;
import java.net.SocketException;
import java.nio.charset.StandardCharsets;
import java.util.function.Consumer;

/**
 * Requester for {@link HelloUDPClient}.
 *
 * <p>Sends requests to the specified address and port.
 *
 * <p>Request data is provided to {@link #accept}.
 *
 * @author Vladimir Eremin
 * @see HelloUDPClient
 * @see SharedUtilities
 */
public class HelloUDPRequester implements Consumer<String> {
  private final DatagramSocket socket;
  private final SocketAddress address;
  private final String format;

  /**
   * Creates a new instance of {@link HelloUDPRequester}.
   *
   * @param socket {@link DatagramSocket} to communicate with.
   * @param address {@link SocketAddress} to send requests to.
   * @param format {@link String} to format response with.
   */
  public HelloUDPRequester(
      final DatagramSocket socket, final SocketAddress address, final String format) {
    this.socket = socket;
    this.address = address;
    this.format = format;
  }

  /**
   * While {@link #socket} not closed, attempts to send the request and receive the response.
   *
   * @param requestData {@link String} to send as request body.
   */
  @Override
  public void accept(final String requestData) {
    final DatagramPacket request = HelloUDPUtilities.makeSendingDatagramPacket(address);
    final DatagramPacket response;
    try {
      response = HelloUDPUtilities.makeReceivingDatagramPacket(socket.getReceiveBufferSize());
    } catch (final SocketException e) {
      return;
    }

    final byte[] requestBytes = requestData.getBytes(StandardCharsets.UTF_8);
    while (!socket.isClosed()) {
      request.setData(requestBytes);
      try {
        socket.send(request);
      } catch (final IOException e) {
        System.err.println("Error occurred while sending a packet. " + e.getMessage());
        continue;
      }

      try {
        socket.receive(response);
      } catch (final IOException e) {
        System.err.println("Error occurred while receiving a packet. " + e.getMessage());
        continue;
      }

      if (!isRequestSettledCorrectly(response, requestData)) {
        System.err.format(
            "Request was not settled correctly.\nExpected: %s\nReceived: %s\n",
            HelloUDPUtilities.GKStringFormat(format, requestData),
            HelloUDPUtilities.getDatagramPacketDataUTF8(response));
        continue;
      }

      break;
    }
  }

  private boolean isRequestSettledCorrectly(
      final DatagramPacket response, final String requestData) {
    return HelloUDPUtilities.getDatagramPacketDataUTF8(response)
        .equals(HelloUDPUtilities.GKStringFormat(format, requestData));
  }
}
