package expression.generic.expression.modes;

import expression.generic.expression.exceptions.DivisionByZeroException;

public class ShortMode extends AbstractMode<Short> {
    @Override
    public final Short add(final Short x, final Short y) {
        return (short) (x + y);
    }

    @Override
    public final Short subtract(final Short x, final Short y) {
        return (short) (x - y);
    }

    @Override
    public final Short multiply(final Short x, final Short y) {
        return (short) (x * y);
    }

    @Override
    public final Short divide(final Short x, final Short y) {
        if (y == 0) {
            throw new DivisionByZeroException("Division by zero.", "/", x + " / " + y);
        }

        return (short) (x / y);
    }

    @Override
    public Short mod(final Short x, final Short y) {
        if (y == 0) {
            throw new DivisionByZeroException("Modulus is zero.", "mod", x + " mod " + y);
        }

        return (short) (x % y);
    }

    @Override
    public final Short constant(final String x) {
        return (short) Integer.parseInt(x);
    }

    @Override
    public Short negate(Short x) {
        return (short) -x;
    }

    @Override
    public Short abs(final Short x) {
        return (short) Math.abs(x);
    }

    @Override
    public Short square(final Short x) {
        return (short) (x * x);
    }
}