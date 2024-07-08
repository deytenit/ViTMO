package info.kgeorgiy.java.advanced.iterative;

import org.junit.jupiter.api.Test;

import java.util.AbstractList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.function.BiFunction;
import java.util.function.Predicate;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/**
 * Full tests for easy version
 * of <a href="https://www.kgeorgiy.info/courses/java-advanced/homeworks.html#homework-concurrent">Iterative parallelism</a> homework
 * for <a href="https://www.kgeorgiy.info/courses/java-advanced/">Java Advanced</a> course.
 *
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public class NewScalarIPTest<P extends NewScalarIP> extends ScalarIPTest<P> {
    protected final int[] steps = {1, 3, 5, 1000_000};

    public NewScalarIPTest() {
    }

    @Test
    public void test91_stepMaximum() throws InterruptedException {
        testStep(Collections::max, NewScalarIP::maximum, COMPARATORS);
    }

    @Test
    public void test92_stepMinimum() throws InterruptedException {
        testStep(Collections::min, NewScalarIP::minimum, COMPARATORS);
    }

    @Test
    public void test93_stepAll() throws InterruptedException {
        testStepS(Stream::allMatch, NewScalarIP::all, PREDICATES);
    }

    @Test
    public void test94_stepAny() throws InterruptedException {
        testStepS(Stream::anyMatch, NewScalarIP::any, PREDICATES);
    }

    @Test
    public void test95_stepCount() throws InterruptedException {
        testStepS((data, value) -> (int) data.filter(value).count(), NewScalarIP::count, PREDICATES);
    }

    @Test
    public void test96_stepPerformance() throws InterruptedException {
        this.<Comparator<Integer>>testStepPerformance(NewScalarIP::maximum, Integer::compare, "maximum");
        this.<Predicate<Integer>>testStepPerformance(NewScalarIP::count, a -> a > 0, "count");
    }

    private <T> void testStepPerformance(
            final StepFunction<P, Integer, T> f,
            final T value,
            final String context
    ) throws InterruptedException {
        final List<Integer> data = randomList(1_000);
        final long denseTime = measureTime(data, 2, f, value, context);
        final long sparseTime = measureTime(data, 2000_000, f, value, context);

        final double ratio = sparseTime / (double) denseTime;
        System.err.format("        %s, performance ratio %.3f (%.1f %.1f)%n", context, ratio, sparseTime / 1e9, denseTime / 1e9);
        checkRatio(ratio, 3);
    }

    private <T> long measureTime(
            final List<Integer> data,
            final int step,
            final StepFunction<P, Integer, T> f,
            final T value,
            final String context
    ) throws InterruptedException {
        return measureTime(
                1,
                sparse(data, step),
                (instance, threads, values) -> {
                    for (int i = 0; i < 3_000; i++) {
                        f.apply(instance, threads, values, value, step);
                    }
                },
                1,
                context + ", step = " + step
        );
    }

    private static <E> List<E> sparse(final List<E> items, final int step) {
        return new AbstractList<>() {
            @Override
            public E get(final int index) {
                return items.get(index / step);
            }

            @Override
            public int size() {
                return items.size() * step;
            }
        };
    }

    protected final <T, U> void testStep(
            final BiFunction<List<Integer>, U, T> fExpected,
            final StepFunction<P, T, U> fActual,
            final List<Named<U>> cases
    ) throws InterruptedException {
        for (final int step : steps) {
            try {
                test(
                        (data, value) -> fExpected.apply(nth(data, step), value),
                        (instance, threads, data, value) -> fActual.apply(instance, threads, data, value, step),
                        cases
                );
            } catch (final AssertionError e) {
                throw new AssertionError("Step " + step + ": " + e.getMessage(), e);
            }
        }
    }

    protected final <T, U> void testStepS(
            final BiFunction<Stream<Integer>, U, T> fExpected,
            final StepFunction<P, T, U> fActual,
            final List<Named<U>> cases
    ) throws InterruptedException {
        testStep((data, value) -> fExpected.apply(data.stream(), value), fActual, cases);
    }

    protected static <T> List<T> nth(final List<T> items, final int step) {
        return IntStream.iterate(0, i -> i < items.size(), i -> i + step).mapToObj(items::get).toList();
    }

    public interface StepFunction<P, T, U> {
        T apply(P instance, int threads, List<Integer> data, U value, int step) throws InterruptedException;
    }
}
