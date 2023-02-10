package expression;

import java.util.Objects;

public class Const implements UniversalExpression {

    private final Number value;

    public Const(int value) {
        this.value = value;
    }

    @Override
    public int getPriority() {
        return 0;
    }

    @Override
    public int evaluate(int x) {
        return value.intValue();
    }

    @Override
    public double evaluate(double x) {
        return value.doubleValue();
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return value.intValue();
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
        return Objects.hash(value);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Const casted = (Const) o;
        return value.equals(casted.value);
    }
}
