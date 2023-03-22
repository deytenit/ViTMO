package expression.generic.expression;

import expression.generic.expression.modes.Mode;

import java.util.Objects;

public class Variable<T extends Number> extends Expression<T> {
    private final String variable;

    public Variable(final String variable, final Mode<T> mode) {
        super(mode);

        this.variable = variable;
    }

    @Override
    public int getPriority() {
        return 0;
    }

    @Override
    public T evaluate(final T x, final T y, final T z) {
        return switch (variable) {
            case "x" -> x;
            case "y" -> y;
            case "z" -> z;
            default -> throw new RuntimeException("Variable evaluation is not possible for " + variable);
        };
    }

    @Override
    public String toString() {
        return variable;
    }

    @Override
    public String toMiniString() {
        return variable;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Variable<?> that = (Variable<?>) o;
        return Objects.equals(variable, that.variable);
    }

    @Override
    public int hashCode() {
        return variable.hashCode();
    }
}
