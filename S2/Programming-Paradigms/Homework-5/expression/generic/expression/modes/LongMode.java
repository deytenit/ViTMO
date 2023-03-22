package expression.generic.expression.modes;

import expression.generic.expression.exceptions.DivisionByZeroException;

public class LongMode extends AbstractMode<Long> {
    @Override
    public final Long add(final Long x, final Long y) {
        return x + y;
    }

    @Override
    public final Long subtract(final Long x, final Long y) {
        return x - y;
    }

    @Override
    public final Long multiply(final Long x, final Long y) {
        return x * y;
    }

    @Override
    public final Long divide(final Long x, final Long y) {
        if (y == 0) {
            throw new DivisionByZeroException("Division by zero.", "/", x + " / " + y);
        }

        return x / y;
    }

    @Override
    public Long mod(final Long x, final Long y) {
        if (y == 0) {
            throw new DivisionByZeroException("Modulus is zero.", "mod", x + " mod " + y);
        }

        return x % y;
    }

    @Override
    public final Long constant(final String x) {
        return Long.parseLong(x);
    }

    @Override
    public Long negate(Long x) {
        return -x;
    }

    @Override
    public Long abs(final Long x) {
        return Math.abs(x);
    }

    @Override
    public Long square(final Long x) {
        return x * x;
    }
}
