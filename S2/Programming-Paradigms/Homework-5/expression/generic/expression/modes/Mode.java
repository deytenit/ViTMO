package expression.generic.expression.modes;

import expression.generic.parser.ExpressionParser;

public interface Mode<T extends Number> {
    ExpressionParser<T> parser();

    T add(final T x, final T y);
    T subtract(final T x, final T y);
    T multiply(final T x, final T y);
    T divide(final T x, final T y);
    T mod(final T x, final T y);

    T constant(final String x);
    T negate(final T x);
    T abs(final T x);
    T square(final T x);

}
