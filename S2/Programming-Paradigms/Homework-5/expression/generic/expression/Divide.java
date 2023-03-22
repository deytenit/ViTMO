package expression.generic.expression;

public class Divide<T extends Number> extends BinaryExpression<T> {
    public static final String OPERATION = "/";

    public Divide(final UniversalExpression<T> lValue, final UniversalExpression<T> rValue) {
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
    protected T calculate(T a, T b) {
        return getMode().divide(a, b);
    }
}
