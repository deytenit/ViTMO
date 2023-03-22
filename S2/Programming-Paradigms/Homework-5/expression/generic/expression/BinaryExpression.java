package expression.generic.expression;

import expression.BitwiseClear;
import expression.BitwiseSet;

import java.util.Objects;

public abstract class BinaryExpression<T extends Number> extends Expression<T> {

    protected final UniversalExpression<T> lValue;
    protected final UniversalExpression<T> rValue;

    public BinaryExpression(final UniversalExpression<T> lValue, final UniversalExpression<T> rValue) {
        super(lValue.getMode());

        this.lValue = lValue;
        this.rValue = rValue;
    }

    public abstract String getOperation();

    protected abstract T calculate(final T a, final T b);

    @Override
    public final T evaluate(final T x, final T y, final T z) {
        return calculate(lValue.evaluate(x, y, z), rValue.evaluate(x, y, z));
    }

    @Override
    public final String toString() {
        return "(" + lValue.toString() + " " + getOperation() + " " + rValue.toString() + ")";
    }

    @Override
    public final String toMiniString() {
        final StringBuilder sb = new StringBuilder();

        if (lValue.getPriority() > getPriority()) {
            sb.append('(').append(lValue.toMiniString()).append(')');
        }
        else {
            sb.append(lValue.toMiniString());
        }

        sb.append(' ').append(getOperation()).append(' ');

        if (rValue.getPriority() >= getPriority() && !associative((BinaryExpression<T>) rValue)) {
            sb.append('(').append(rValue.toMiniString()).append(')');
        }
        else {
            sb.append(rValue.toMiniString());
        }

        return sb.toString();
    }

    @Override
    public boolean equals(final Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        BinaryExpression<?> that = (BinaryExpression<?>) o;

        return Objects.equals(lValue, that.lValue)
                && Objects.equals(rValue, that.rValue)
                && getOperation().equals(that.getOperation());
    }

    @Override
    public int hashCode() {
        return Objects.hash(lValue, rValue, getOperation());
    }

    private boolean associative(final BinaryExpression<T> following) {
        return (!getOperation().equals(Multiply.OPERATION) || following.getOperation().equals(Multiply.OPERATION))
                && (!getOperation().equals(Subtract.OPERATION) || !following.getOperation().equals(Subtract.OPERATION))
                && !following.getOperation().equals(BitwiseSet.OPERATION)
                && !following.getOperation().equals(BitwiseClear.OPERATION)
                && (!getOperation().equals(Subtract.OPERATION) || !following.getOperation().equals(Add.OPERATION))
                && !getOperation().equals(Divide.OPERATION);
    }
}
