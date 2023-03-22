package expression.generic;

import java.util.Arrays;

public final class Main {
    public static final int EVAL_RANGE_MIN = -2;
    public static final int EVAL_RANGE_MAX = 2;

    public static void main(final String... args) throws Exception {
        final Tabulator tabulator = new GenericTabulator();

        if (args.length < 2) {
            throw new IllegalArgumentException(String.format("Expected two arguments, found %s.", args.length));
        }

        System.out.println(
                Arrays.deepToString(tabulator.tabulate(
                        args[0],
                        args[1],
                        EVAL_RANGE_MIN,
                        EVAL_RANGE_MAX,
                        EVAL_RANGE_MIN,
                        EVAL_RANGE_MAX,
                        EVAL_RANGE_MIN,
                        EVAL_RANGE_MAX
                ))
        );
    }
}
