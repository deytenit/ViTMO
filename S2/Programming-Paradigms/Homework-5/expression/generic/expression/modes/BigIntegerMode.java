package expression.generic.expression.modes;

import expression.generic.expression.exceptions.DivisionByZeroException;
import expression.generic.expression.exceptions.ExpressionEvaluationException;

import java.math.BigInteger;

public class BigIntegerMode extends AbstractMode<BigInteger> {
    @Override
    public final BigInteger add(final BigInteger x, final BigInteger y) {
        return x.add(y);
    }

    @Override
    public final BigInteger subtract(final BigInteger x, final BigInteger y) {
        return x.subtract(y);
    }

    @Override
    public final BigInteger multiply(final BigInteger x, final BigInteger y) {
        return x.multiply(y);
    }

    @Override
    public final BigInteger divide(final BigInteger x, final BigInteger y) {
        try {
            return x.divide(y);
        }
        catch (final ArithmeticException error) {
            throw new DivisionByZeroException("Division by zero.", "/", x + " / " + y);
        }
    }

    @Override
    public BigInteger mod(final BigInteger x, final BigInteger y) {
        if (y.compareTo(BigInteger.ZERO) <= 0) {
            throw new ExpressionEvaluationException("Modulus not positive.", "mod", x + " mod " + y);
        }

        return x.mod(y);
    }

    @Override
    public final BigInteger constant(final String x) {
        return new BigInteger(x);
    }

    @Override
    public BigInteger negate(BigInteger x) {
        return x.negate();
    }

    @Override
    public BigInteger abs(final BigInteger x) {
        return x.abs();
    }

    @Override
    public BigInteger square(final BigInteger x) {
        return x.multiply(x);
    }
}
