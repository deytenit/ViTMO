package info.kgeorgiy.ja.eremin.iterative;

import info.kgeorgiy.java.advanced.mapper.ParallelMapper;
import java.util.*;
import java.util.function.Function;
import java.util.stream.IntStream;

/**
 * Multi-consumer, concurrent {@link List} mapping capability with fixed amount of {@link Thread
 * Threads} workers.
 *
 * @author Vladimir Eremin
 */
@SuppressWarnings("unused")
public class ParallelMapperImpl implements ParallelMapper {

  private final Queue<Runnable> queue;
  private final List<Thread> threading;
  private volatile boolean closed;

  /**
   * Creates instance of {@link ParallelMapperImpl} with specified amount of threads, those shall be
   * created.
   *
   * @param threads Amount of threads to operate on.
   */
  public ParallelMapperImpl(final int threads) {
    queue = new ArrayDeque<>();
    threading =
        IntStream.range(0, threads)
            .mapToObj(
                i -> {
                  final Thread thread = new Thread(this::threadAction);
                  thread.start();
                  return thread;
                })
            .toList();
    closed = false;
  }

  private void threadAction() {
    try {
      while (!Thread.interrupted() && !closed) {
        Runnable f;
        synchronized (queue) {
          while (queue.isEmpty()) {
            queue.wait();
          }

          f = queue.poll();
        }
        f.run();
      }
    } catch (final InterruptedException ignored) {
    } finally {
      Thread.currentThread().interrupt();
    }
  }

  /**
   * Concurrently creates a new {@link List} populated with the results of calling a provided {@link
   * Function} on every element in the calling List.
   *
   * <p>{@inheritDoc}
   *
   * @param f {@link Function} that maps elements of {@code args}.
   * @param args {@link List} containing elements on which {@code f} would be called.
   * @param <T> Calling list ({@code args}) data type.
   * @param <R> Resulting list data type.
   * @return A new list with each element being the result of {@code f} function.
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T, R> List<R> map(final Function<? super T, ? extends R> f, final List<? extends T> args)
      throws InterruptedException {
    if (closed) {
      throw new InterruptedException("Mapper is closed. All threads are interrupted.");
    }

    final ResultHolder<R> result = new ResultHolder<>(args.size());

    IntStream.range(0, result.size())
        .forEach(
            i -> {
              synchronized (queue) {
                queue.add(
                    () -> {
                      R res = null;
                      try {
                        res = f.apply(args.get(i));
                      } catch (final RuntimeException postponed) {
                        result.except(postponed);
                      }

                      result.set(i, res);
                    });

                queue.notify();
              }
            });

    return result.retrieve();
  }

  /**
   * Tries to interrupt and join all threads created by {@code this} instance.
   *
   * <p>{@inheritDoc}
   */
  @Override
  public void close() {
    closed = true;
    threading.forEach(Thread::interrupt);
    for (final Thread thread : threading) {
      while (true) {
        try {
          thread.join();
          break;
        } catch (final InterruptedException ignored) {
        }
      }
    }
  }

  private class ResultHolder<T> {
    private final int size;
    private final List<T> data;

    private int consumed;
    private RuntimeException exception;

    public ResultHolder(final int size) {
      this.size = size;
      data = new ArrayList<>(Collections.nCopies(size, null));
      consumed = 0;
      exception = null;
    }

    public int size() {
      return size;
    }

    public synchronized void except(final RuntimeException e) {
      exception = e;
      consumed = size;

      notify();
    }

    public synchronized void set(final int pos, final T value) {
      data.set(pos, value);

      if (++consumed == size) {
        notify();
      }
    }

    public synchronized List<T> retrieve() throws InterruptedException {
      while (!closed && consumed != size) {
        wait();
      }

      if (consumed != size) {
        throw new InterruptedException("Mapper was closed. Mapping was not completed.");
      }

      if (exception != null) {
        throw exception;
      }

      return data;
    }
  }
}
