package expression;

import java.util.Objects;

public class Negate extends UnaryExpression {
    public static final String OPERATION = "-";

    public Negate(UniversalExpression value) {
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
        return -a;
    }

    @Override
    protected double calculate(double a) {
        return -a;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Negate casted = (Negate) o;
        return Objects.equals(value, casted.value);
    }
}
