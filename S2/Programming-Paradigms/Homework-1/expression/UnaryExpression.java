package expression;

import java.util.Objects;

public abstract class UnaryExpression implements UniversalExpression {
    protected final UniversalExpression value;

    public UnaryExpression(UniversalExpression value) {
        this.value = value;
    }

    public abstract String getOperation();

    @Override
    public int evaluate(int x) {
        return calculate(value.evaluate(x));
    }

    @Override
    public double evaluate(double x) {
        return calculate(value.evaluate(x));
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return calculate(value.evaluate(x, y, z));
    }

    protected abstract int calculate(int a);

    protected abstract double calculate(double b);

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
    public int hashCode() {
        return Objects.hash(value, getOperation());
    }
}
