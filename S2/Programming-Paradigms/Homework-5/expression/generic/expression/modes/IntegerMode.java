package expression.generic.expression.modes;

import expression.generic.expression.exceptions.ExpressionEvaluationException;
import expression.generic.expression.exceptions.DivisionByZeroException;
import expression.generic.expression.exceptions.OverflowException;
import expression.generic.expression.exceptions.UnderflowException;

public class IntegerMode extends AbstractMode<Integer> {

    final boolean checkOverflow;

    public IntegerMode(final boolean checkOverflow) {
        this.checkOverflow = checkOverflow;
    }

    @Override
    public final Integer add(final Integer x, final Integer y) {
        if (checkOverflow) {
            if (y > 0 && x > Integer.MAX_VALUE - y) {
                throw new OverflowException("Overflow.", "+", x + " + " + y);
            }

            if (y < 0 && x < Integer.MIN_VALUE - y) {
                throw new UnderflowException("Underflow.", "+", x + " + " + y);
            }
        }

        return x + y;
    }

    @Override
    public final Integer subtract(final Integer x, final Integer y) {
        if (checkOverflow) {
            if (y < 0 && x > Integer.MAX_VALUE + y) {
                throw new OverflowException("Overflow.", "-", x + " - " + y);
            }

            if (y > 0 && x < Integer.MIN_VALUE + y) {
                throw new UnderflowException("Underflow.", "-", x + " - " + y);
            }
        }

        return x - y;
    }

    @Override
    public final Integer multiply(final Integer x, final Integer y) {
        final int result = x * y;

        if (checkOverflow) {
            checkMultiplicationOverflow(x, y);
        }

        return result;
    }

    @Override
    public final Integer divide(final Integer x, final Integer y) {
        if (y == 0) {
            throw new DivisionByZeroException("Division by zero.", "/", x + " / " + y);
        }

        if (checkOverflow && y == -1 && x == Integer.MIN_VALUE) {
            throw new OverflowException("Overflow.", "/", x + " / " + y);
        }

        return x / y;
    }

    @Override
    public Integer mod(final Integer x, final Integer y) {
        if (y == 0) {
            throw new DivisionByZeroException("Division by zero.", "/", x + " / " + y);
        }

        return x % y;
    }

    @Override
    public final Integer constant(final String x) {
        try {
            return Integer.parseInt(x);
        }
        catch (final NumberFormatException error) {
            if (x.matches("-?\\d+")) {
                if (x.startsWith("-")) {
                    throw new UnderflowException("Underflow.", "const", x);
                }

                throw new OverflowException("Overflow.", "const", x);
            }

            throw error;
        }
    }

    @Override
    public Integer negate(Integer x) {
        if (checkOverflow && x == Integer.MIN_VALUE) {
            throw new ExpressionEvaluationException("Overflow.", "-", "- " + x);
        }

        return -x;
    }

    @Override
    public Integer abs(final Integer x) {
        if (checkOverflow && x == Integer.MIN_VALUE) {
            throw new ExpressionEvaluationException("Overflow.", "-", "- " + x);
        }

        return Math.abs(x);
    }

    @SuppressWarnings("SuspiciousNameCombination")
    @Override
    public Integer square(final Integer x) {
        if (checkOverflow) {
            checkMultiplicationOverflow(x, x);
        }

        return x * x;
    }

    private void checkMultiplicationOverflow(final int x, final int y) {
        final int result = x * y;

        if (x != 0 && result / x != y
                || (x > 0 && y > 0 || x < 0 && y < 0) && result < 0
                || (x < 0 && y > 0 || x > 0 && y < 0) && result > 0) {
            if (x > 0 && y > 0 || x < 0 && y < 0) {
                throw new OverflowException("Overflow.", "*", x + " * " + y);
            }
            else {
                throw new ExpressionEvaluationException("Underflow.", "*", x + " * " + y);
            }
        }
    }
}
