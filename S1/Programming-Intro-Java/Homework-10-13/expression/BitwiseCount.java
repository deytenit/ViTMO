package expression;

import java.util.Objects;

public class BitwiseCount extends UnaryExpression {
    public static final String OPERATION = "count";

    public BitwiseCount(UniversalExpression value) {
        super(value);
    }

    @Override
    public String getOperation() {
        return OPERATION;
    }

    @Override
    public int getPriority() {
        return 0;
    }

    @Override
    protected int calculate(int a) {
        return Integer.bitCount(a);
    }

    @Override
    protected double calculate(double b) {
        return Integer.bitCount((int) b);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        BitwiseCount casted = (BitwiseCount) o;
        return Objects.equals(value, casted.value);
    }
}
