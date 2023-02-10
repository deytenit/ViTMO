package expression;

import java.util.Objects;

public class Multiply extends BinaryExpression {
    public static final String OPERATION = "*";

    public Multiply(UniversalExpression lValue, UniversalExpression rValue) {
        super(lValue, rValue);
    }

    @Override
    public String getOperation() {
        return OPERATION;
    }

    @Override
    public int getPriority() {
        return 1;
    }

    @Override
    protected int calculate(int a, int b) {
        return a * b;
    }

    @Override
    protected double calculate(double a, double b) {
        return a * b;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Multiply casted = (Multiply) o;
        return Objects.equals(lValue, casted.lValue) && Objects.equals(rValue, casted.rValue);
    }
}
