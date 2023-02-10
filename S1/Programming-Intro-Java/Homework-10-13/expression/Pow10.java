package expression;

import java.util.Objects;

public class Pow10 extends UnaryExpression {
    public static final String OPERATION = "pow10";

    public Pow10(UniversalExpression value) {
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
        int res = 1;

        for (int i = 0; i < a; ++i) {
            res *= 10;
        }

        return res;
    }

    @Override
    protected double calculate(double a) {
        return calculate((int) a);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Pow10 casted = (Pow10) o;
        return Objects.equals(value, casted.value);
    }
}
