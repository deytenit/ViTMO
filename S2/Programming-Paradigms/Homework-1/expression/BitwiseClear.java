package expression;

import java.util.Objects;

public class BitwiseClear extends BinaryExpression {
    public static final String OPERATION = "clear";

    public BitwiseClear(UniversalExpression lValue, UniversalExpression rValue) {
        super(lValue, rValue);
    }

    @Override
    public int getPriority() {
        return 3;
    }

    @Override
    public String getOperation() {
        return OPERATION;
    }

    @Override
    protected int calculate(int a, int b) {
        return a & ~(1 << b);
    }

    @Override
    protected double calculate(double a, double b) {
        return (int) a & ~(1 << (int) b);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        BitwiseClear casted = (BitwiseClear) o;
        return Objects.equals(lValue, casted.lValue) && Objects.equals(rValue, casted.rValue);
    }
}
