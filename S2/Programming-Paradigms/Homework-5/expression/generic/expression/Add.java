package expression.generic.expression;

public class Add<T extends Number> extends BinaryExpression<T> {
    public static final String OPERATION = "+";

    public Add(final UniversalExpression<T> lValue, final UniversalExpression<T> rValue) {
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
    protected T calculate(T a, T b) {
        return getMode().add(a, b);
    }
}
