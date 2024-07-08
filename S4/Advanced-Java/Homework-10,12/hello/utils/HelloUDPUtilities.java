package info.kgeorgiy.ja.eremin.hello.utils;

import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import java.io.Closeable;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketAddress;
import java.net.SocketException;
import java.nio.charset.StandardCharsets;

public class HelloUDPUtilities {
  /**
   * Character that is used as a marker for formatting.
   *
   * @see #GKStringFormat(String, String)
   */
  public static final String FORMAT_MARKER = "$";

  /** Default format for the response message. */
  public static final String DEFAULT_FORMAT = "Hello, $";

  /** Default prefix for the request message. */
  public static final String DEFAULT_PREFIX = "anonymous";

  /** Default buffer capacity for the {@link java.nio.ByteBuffer}. */
  public static final int DEFAULT_BUFFER_CAPACITY = 500;

  /**
   * Creates a new {@link DatagramSocket} with a random port, and sets the timeout to {@link
   * SharedUtilities#WATERFALL_TIMEOUT}.
   *
   * @return new {@link DatagramSocket}.
   * @throws SocketException if an error occurs while creating the socket.
   */
  public static DatagramSocket createDatagramSocketWithTimeout() throws SocketException {
    return createDatagramSocketWithTimeout(0);
  }

  /**
   * Creates a new {@link DatagramSocket} with a specified port, and sets the timeout to {@link
   * SharedUtilities#WATERFALL_TIMEOUT}.
   *
   * @return new {@link DatagramSocket}.
   * @throws SocketException if an error occurs while creating the socket.
   */
  public static DatagramSocket createDatagramSocketWithTimeout(final int port)
      throws SocketException {
    final DatagramSocket socket = new DatagramSocket(port);
    socket.setSoTimeout(SharedUtilities.WATERFALL_TIMEOUT);

    return socket;
  }

  /**
   * Formats the provided {@link String} with the provided format, using #FORMAT_MARKER as a
   * placeholder.
   *
   * @param format format to use.
   * @param data data to insert.
   * @return formatted string.
   */
  public static String GKStringFormat(final String format, final String data) {
    return format.replace(FORMAT_MARKER, data);
  }

  /**
   * Makes {@link DatagramPacket} for receiving.
   *
   * <p>Utility function that creates a DatagramPacket that is ready for receiving.
   *
   * @param receiveBufferSize size of the buffer of the DatagramPacket.
   * @return new DatagramPacket for receiving.
   */
  public static DatagramPacket makeReceivingDatagramPacket(final int receiveBufferSize) {
    return new DatagramPacket(new byte[receiveBufferSize], receiveBufferSize);
  }

  /**
   * Makes {@link DatagramPacket} for sending.
   *
   * <p>Utility function that creates an empty DatagramPacket that is ready for sending.
   *
   * @param address address to send to.
   * @return new DatagramPacket for sending.
   */
  public static DatagramPacket makeSendingDatagramPacket(final SocketAddress address) {
    return new DatagramPacket(new byte[0], 0, address);
  }

  /**
   * Gets UTF-8 string from {@link DatagramPacket}.
   *
   * <p>Utility function that creates a new UTF-8 string from the data of the provided
   * DatagramPacket.
   *
   * @param packet DatagramPacket to get data from.
   * @return UTF-8 string from the data of the provided DatagramPacket.
   */
  public static String getDatagramPacketDataUTF8(final DatagramPacket packet) {
    return new String(
        packet.getData(), packet.getOffset(), packet.getLength(), StandardCharsets.UTF_8);
  }

  /**
   * Closes resource, ignoring any exceptions.
   *
   * @param o {@link Closeable} object.
   */
  public static void closeIgnoring(final Closeable o) {
    try {
      o.close();
    } catch (final Exception ignored) {
    }
  }

  /**
   * Context of the channel.
   *
   * <p>Contains the index of the channel and the number of successful requests that were sent from
   * it.
   */
  public static class ChannelContext {
    private final int index;
    private int requests;

    /**
     * Creates a new instance of {@link ChannelContext}.
     *
     * @param index index of the channel.
     * @param requests initial number of requests.
     */
    public ChannelContext(final int index, final int requests) {
      this.index = index;
      this.requests = requests;
    }

    /**
     * Gets the index of the channel.
     *
     * @return index of the channel.
     */
    public int index() {
      return index;
    }

    /**
     * Gets the number of requests that were sent from the channel.
     *
     * @return number of requests that were sent from the channel.
     */
    public int requests() {
      return requests;
    }

    /** Increments the number of requests that were sent from the channel. */
    public void inc() {
      ++requests;
    }
  }
}
