package expression;

import java.util.Objects;

public class Subtract extends BinaryExpression {
    public static final String OPERATION = "-";

    public Subtract(UniversalExpression lValue, UniversalExpression rValue) {
        super(lValue, rValue);
    }

    @Override
    public String getOperation() {
        return OPERATION;
    }

    @Override
    public int getPriority() {
        return 2;
    }

    @Override
    protected int calculate(int a, int b) {
        return a - b;
    }

    @Override
    protected double calculate(double a, double b) {
        return a - b;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Subtract casted = (Subtract) o;
        return Objects.equals(lValue, casted.lValue) && Objects.equals(rValue, casted.rValue);
    }
}
