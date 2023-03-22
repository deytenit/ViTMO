package expression.generic.expression;

import java.util.Objects;

public abstract class UnaryExpression<T extends Number> extends Expression<T> {
    protected final UniversalExpression<T> value;

    public UnaryExpression(UniversalExpression<T> value) {
        super(value.getMode());

        this.value = value;
    }

    public abstract String getOperation();

    protected abstract T calculate(final T a);

    @Override
    public T evaluate(final T x, final T y, final T z) {
        return calculate(value.evaluate(x, y, z));
    }

    @Override
    public String toString() {
        return getOperation() + "(" + value.toString() + ")";
    }

    @Override
    public String toMiniString() {
        return value.getPriority() == 0 ? getOperation() + " " + value.toMiniString()
                : getOperation() + "(" + value.toMiniString() + ")";
    }

    @Override
    public boolean equals(final Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        UnaryExpression<?> that = (UnaryExpression<?>) o;

        return Objects.equals(value, that.value);
    }

    @Override
    public int hashCode() {
        return Objects.hash(value, getOperation());
    }
}
