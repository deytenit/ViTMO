package expression.generic.expression;

import expression.generic.expression.modes.Mode;

import java.util.Objects;

public class Const<T extends Number> extends Expression<T> {

    private final T value;

    public Const(final String value, final Mode<T> mode) {
        super(mode);

        this.value = mode.constant(value);
    }

    @Override
    public int getPriority() {
        return 0;
    }

    @Override
    public T evaluate(T x, T y, T z) {
        return value;
    }

    @Override
    public String toString() {
        return value.toString();
    }

    @Override
    public String toMiniString() {
        return value.toString();
    }

    @Override
    public int hashCode() {
        return Objects.hash(value, mode);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Const<?> that = (Const<?>) o;
        return value.equals(that.value);
    }
}
