package info.kgeorgiy.ja.eremin.iterative;

import info.kgeorgiy.java.advanced.iterative.NewScalarIP;
import info.kgeorgiy.java.advanced.mapper.ParallelMapper;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/**
 * Concurrent {@link List List's} iterative capabilities
 *
 * @author Vladimir Eremin
 */
@SuppressWarnings("unused")
public class IterativeParallelism implements NewScalarIP {
  private final ParallelMapper mapper;

  /**
   * Constructs default instance of {@link IterativeParallelism} with flex {@link Thread Threads}
   * amount.
   */
  public IterativeParallelism() {
    mapper = null;
  }

  /**
   * Constructs instance of {@link IterativeParallelism} with behaviour defined by {@link
   * ParallelMapper} provided.
   *
   * @param mapper {@link ParallelMapper} custom concurrent computation engine.
   * @apiNote {@link Thread Threads} amount may be defined by {@code mapper}, and though may be
   *     ignored at method calls.
   */
  public IterativeParallelism(final ParallelMapper mapper) {
    this.mapper = mapper;
  }

  private static <T, U> List<U> defaultMapper(
      final Function<Stream<? extends T>, U> reduceFn, final List<Stream<? extends T>> blocks)
      throws InterruptedException {
    final int blocksAmount = blocks.size();
    List<U> result;
    Thread[] threading = new Thread[blocksAmount];

    result = new ArrayList<>(Collections.nCopies(blocksAmount, null));

    for (int i = 0; i < blocksAmount; ++i) {
      final int finalI = i;
      threading[i] = new Thread(() -> result.set(finalI, reduceFn.apply(blocks.get(finalI))));
      threading[i].start();
    }

    for (final Thread thread : threading) {
      thread.join();
    }

    return result;
  }

  private static <T> List<Stream<? extends T>> genBlocks(
      final int threads, final int step, final List<? extends T> values) {
    final int blockSize = values.size() / threads;
    int leftover = values.size() % threads;

    List<Stream<? extends T>> blocks = new ArrayList<>();

    for (int i = 0, left = 0; i < threads; ++i) {
      final int right = left + blockSize + (leftover-- > 0 ? 1 : 0);
      final int steppedLeft = left % step == 0 ? left : left + step - (left % step);

      if (steppedLeft < right) {
        blocks.add(
            IntStream.iterate(steppedLeft, j -> j < right, j -> j + step).mapToObj(values::get));
      }

      left = right;
    }

    return blocks;
  }

  private <T, U> U reduce(
      final int threads,
      final List<? extends T> values,
      final Function<Stream<? extends T>, U> reduceFn,
      final Function<Stream<? extends U>, U> joinFn,
      final int step)
      throws InterruptedException {
    final List<Stream<? extends T>> blocks =
        genBlocks(Math.min(values.size(), threads), step, values);
    final int blocksAmount = blocks.size();

    List<U> result;

    if (mapper != null) {
      result = mapper.map(reduceFn, blocks);
    } else {
      result = defaultMapper(reduceFn, blocks);
    }

    return joinFn.apply(result.stream());
  }

  /**
   * Concurrently finds maximum token of provided {@code values}, taking every {@code step} element.
   *
   * @param threads number of concurrent threads.
   * @param values values to get maximum of.
   * @param comparator token comparator.
   * @param step step size.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> T maximum(
      final int threads,
      final List<? extends T> values,
      final Comparator<? super T> comparator,
      final int step)
      throws InterruptedException {
    return reduce(
        threads,
        values,
        stream -> stream.max(comparator).orElse(null),
        stream -> stream.max(comparator).orElse(null),
        step);
  }

  /**
   * Concurrently finds minimum token of provided {@code values}, taking every {@code step} element.
   *
   * @param threads number of concurrent threads.
   * @param values values to get minimum of.
   * @param comparator token comparator.
   * @param step step size.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> T minimum(
      final int threads,
      final List<? extends T> values,
      final Comparator<? super T> comparator,
      final int step)
      throws InterruptedException {
    return maximum(threads, values, Collections.reverseOrder(comparator), step);
  }

  /**
   * Concurrently test {@code predicate} satisfaction across all provided {@code values}, taking
   * every {@code step} element.
   *
   * @param threads number of concurrent threads.
   * @param values values to test.
   * @param predicate test predicate.
   * @param step step size.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> boolean all(
      final int threads,
      final List<? extends T> values,
      final Predicate<? super T> predicate,
      final int step)
      throws InterruptedException {
    return reduce(
        threads,
        values,
        stream -> stream.allMatch(predicate),
        stream -> stream.allMatch(Boolean::booleanValue),
        step);
  }

  /**
   * Concurrently test at least one {@code predicate} satisfaction out of provided {@code values},
   * taking every {@code step} element.
   *
   * @param threads number of concurrent threads.
   * @param values values to test.
   * @param predicate test predicate.
   * @param step step size.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> boolean any(
      final int threads,
      final List<? extends T> values,
      final Predicate<? super T> predicate,
      final int step)
      throws InterruptedException {
    return !all(threads, values, Predicate.not(predicate), step);
  }

  /**
   * Concurrently counts number of provided {@code values} satisfying {@code predicate}, taking
   * every {@code step} element.
   *
   * @param threads number of concurrent threads.
   * @param values values to test.
   * @param predicate test predicate.
   * @param step step size.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> int count(
      final int threads,
      final List<? extends T> values,
      final Predicate<? super T> predicate,
      final int step)
      throws InterruptedException {
    return reduce(
        threads,
        values,
        stream -> (int) stream.filter(predicate).count(),
        stream -> stream.mapToInt(Integer::valueOf).sum(),
        step);
  }

  /**
   * Concurrently finds maximum token of provided {@code values}.
   *
   * @param threads number of concurrent threads.
   * @param values values to get maximum of.
   * @param comparator token comparator.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> T maximum(
      final int threads, final List<? extends T> values, final Comparator<? super T> comparator)
      throws InterruptedException {
    return maximum(threads, values, comparator, 1);
  }

  /**
   * Concurrently finds minimum token of provided {@code values}.
   *
   * @param threads number of concurrent threads.
   * @param values values to get minimum of.
   * @param comparator token comparator.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> T minimum(
      final int threads, final List<? extends T> values, final Comparator<? super T> comparator)
      throws InterruptedException {
    return minimum(threads, values, comparator, 1);
  }

  /**
   * Concurrently test {@code predicate} satisfaction across all provided {@code values}.
   *
   * @param threads number of concurrent threads.
   * @param values values to test.
   * @param predicate test predicate.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> boolean all(
      final int threads, final List<? extends T> values, final Predicate<? super T> predicate)
      throws InterruptedException {
    return all(threads, values, predicate, 1);
  }

  /**
   * Concurrently test at least one {@code predicate} satisfaction out of provided {@code values}.
   *
   * @param threads number of concurrent threads.
   * @param values values to test.
   * @param predicate test predicate.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> boolean any(
      final int threads, final List<? extends T> values, final Predicate<? super T> predicate)
      throws InterruptedException {
    return any(threads, values, predicate, 1);
  }

  /**
   * Concurrently counts number of provided {@code values} satisfying {@code predicate}.
   *
   * @param threads number of concurrent threads.
   * @param values values to test.
   * @param predicate test predicate.
   * @param <T> {@inheritDoc}
   * @return {@inheritDoc}
   * @throws InterruptedException {@inheritDoc}
   */
  @Override
  public <T> int count(
      final int threads, final List<? extends T> values, final Predicate<? super T> predicate)
      throws InterruptedException {
    return count(threads, values, predicate, 1);
  }
}
