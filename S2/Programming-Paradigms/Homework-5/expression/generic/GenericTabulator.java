package expression.generic;

import expression.generic.expression.TripleExpression;
import expression.generic.expression.exceptions.ExpressionEvaluationException;
import expression.generic.expression.exceptions.ExpressionParsingException;
import expression.generic.expression.modes.*;

import java.util.Map;

public class GenericTabulator implements Tabulator {

    private final Map<String, Mode<?>> modes = Map.of(
            "i", new IntegerMode(true),
            "d", new DoubleMode(),
            "bi", new BigIntegerMode(),
            "u", new IntegerMode(false),
            "l", new LongMode(),
            "s", new ShortMode()
    );

    @Override
    public Object[][][] tabulate(
            String mode,
            String expression,
            int x1,
            int x2,
            int y1,
            int y2,
            int z1,
            int z2
    ) throws Exception {
        if (!modes.containsKey(mode)) {
            throw new ModeSelectorException(String.format("Mode '%s' does not exists.", mode));
        }

        final Mode<?> selected = modes.get(mode);

        return evaluateRange(selected, expression, x1, x2, y1, y2, z1, z2);
    }

    private static <T extends Number> Object[][][] evaluateRange(
            final Mode<T> mode,
            final String expression,
            final int x1,
            final int x2,
            final int y1,
            final int y2,
            final int z1,
            final int z2
    ) throws ExpressionParsingException {
        final TripleExpression<T> parsed = mode.parser().parse(expression);

        Object[][][] result = new Object[x2 - x1 + 1][y2 - y1 + 1][z2 - z1 + 1];
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                for (int k = z1; k <= z2; k++) {
                    try {
                        result[i - x1][j - y1][k - z1] = parsed.evaluate(
                                mode.constant(String.valueOf(i)),
                                mode.constant(String.valueOf(j)),
                                mode.constant(String.valueOf(k))
                        );
                    } catch (ExpressionEvaluationException error) {
                        result[i - x1][j - y1][k - z1] = null;
                    }
                }
            }
        }

        return result;
    }
}
