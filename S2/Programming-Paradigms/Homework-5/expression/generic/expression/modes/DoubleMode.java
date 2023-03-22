package expression.generic.expression.modes;

public class DoubleMode extends AbstractMode<Double> {
    @Override
    public final Double add(final Double x, final Double y) {
        return x + y;
    }

    @Override
    public final Double subtract(final Double x, final Double y) {
        return x - y;
    }

    @Override
    public final Double multiply(final Double x, final Double y) {
        return x * y;
    }

    @Override
    public final Double divide(final Double x, final Double y) {
        return x / y;
    }

    @Override
    public Double mod(final Double x, final Double y) {
        return x % y;
    }

    @Override
    public final Double constant(final String x) {
        return Double.parseDouble(x);
    }

    @Override
    public Double negate(Double x) {
        return -x;
    }

    @Override
    public Double abs(final Double x) {
        return Math.abs(x);
    }

    @Override
    public Double square(final Double x) {
        return x * x;
    }
}
