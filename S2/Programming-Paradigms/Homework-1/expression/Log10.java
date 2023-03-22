package expression;

import java.util.Objects;

public class Log10 extends UnaryExpression {
    public static final String OPERATION = "log10";

    public Log10(UniversalExpression value) {
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
        int res = 0;

        while (a >= 10) {
            ++res;
            a /= 10;
        }

        return res;
    }

    @Override
    protected double calculate(double b) {
        return calculate((int) b);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Log10 casted = (Log10) o;
        return Objects.equals(value, casted.value);
    }
}
