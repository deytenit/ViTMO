package info.kgeorgiy.java.advanced.iterative;

import org.junit.jupiter.api.Test;

import java.util.Comparator;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.stream.Collectors;

/**
 * Full tests for hard version
 * of <a href="https://www.kgeorgiy.info/courses/java-advanced/homeworks.html#homework-concurrent">Iterative parallelism</a> homework
 * for <a href="https://www.kgeorgiy.info/courses/java-advanced/">Java Advanced</a> course.
 *
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public class NewListIPTest<P extends NewListIP> extends NewScalarIPTest<P> {
    public NewListIPTest() {
    }

    @Test
    public void test10_sleepPerformance() throws InterruptedException {
        testPerformance("filter", MAX_THREADS, 5, 0, 1.5, (instance, threads, values) ->
                instance.filter(threads, values, v -> sleep(v % 3 == 1), 1));
        testPerformance("map", MAX_THREADS, 5, 0, 1.5, (instance, threads, values) ->
                instance.map(threads, values, v -> sleep(v + 10), 1));
    }

    @Test
    public void test51_join() throws InterruptedException {
        testStepS(
                (data, ignore) -> data.map(Object::toString).collect(Collectors.joining()),
                (i, t, d, v, s) -> i.join(t, d, s),
                UNIT
        );
    }

    @Test
    public void test52_filter() throws InterruptedException {
        testStepS(
                (data, predicate) -> data.filter(predicate).toList(),
                NewListIP::filter,
                PREDICATES
        );
    }

    @Test
    public void test53_map() throws InterruptedException {
        testStepS((data, f) -> data.map(f).toList(), NewListIP::map, FUNCTIONS);
    }

    @Test
    public void test54_mapMaximum() throws InterruptedException {
        testStepS(
                (data, f) -> data.map(f).map(Objects::toString).max(Comparator.naturalOrder()),
                (instance, threads, data, f, step) -> {
                    final List<String> mapped = instance.map(threads, data, f.andThen(Objects::toString), step);
                    return Optional.of(instance.maximum(threads, mapped, Comparator.naturalOrder()));
                },
                FUNCTIONS
        );
    }
}
