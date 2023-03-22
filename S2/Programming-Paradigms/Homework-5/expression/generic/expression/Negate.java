package expression.generic.expression;

public class Negate<T extends Number> extends UnaryExpression<T> {
    public static final String OPERATION = "-";

    public Negate(final UniversalExpression<T> value) {
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
    protected T calculate(final T x) {
        return getMode().negate(x);
    }
}
