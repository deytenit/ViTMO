package expression;

import java.util.Objects;

public abstract class BinaryExpression implements UniversalExpression {

    protected final UniversalExpression lValue;
    protected final UniversalExpression rValue;

    public BinaryExpression(UniversalExpression lValue, UniversalExpression rValue) {
        this.lValue = lValue;
        this.rValue = rValue;
    }

    public abstract String getOperation();

    @Override
    public int evaluate(int x) {
        return calculate(lValue.evaluate(x), rValue.evaluate(x));
    }

    @Override
    public double evaluate(double x) {
        return calculate(lValue.evaluate(x), rValue.evaluate(x));
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return calculate(lValue.evaluate(x, y, z), rValue.evaluate(x, y, z));
    }

    protected abstract int calculate(int a, int b);

    protected abstract double calculate(double a, double b);

    @Override
    public String toString() {
        return "(" + lValue.toString() + " " + getOperation() + " " + rValue.toString() + ")";
    }

    public String toMiniString() {
        StringBuilder tmp = new StringBuilder();

        if (lValue.getPriority() > getPriority()) {
            tmp.append('(').append(lValue.toMiniString()).append(')');
        }
        else {
            tmp.append(lValue.toMiniString());
        }

        tmp.append(' ').append(getOperation()).append(' ');

        if (rValue.getPriority() >= getPriority() && !associative((BinaryExpression) rValue)) {
            tmp.append('(').append(rValue.toMiniString()).append(')');
        }
        else {
            tmp.append(rValue.toMiniString());
        }

        return tmp.toString();
    }

    @Override
    public int hashCode() {
        return Objects.hash(lValue, rValue, getOperation());
    }

    private boolean associative(BinaryExpression following) {
        return (!getOperation().equals(Multiply.OPERATION) || following.getOperation().equals(Multiply.OPERATION))
                && (!getOperation().equals(Subtract.OPERATION) || !following.getOperation().equals(Subtract.OPERATION))
                && !following.getOperation().equals(BitwiseSet.OPERATION)
                && !following.getOperation().equals(BitwiseClear.OPERATION)
                && (!getOperation().equals(Subtract.OPERATION) || !following.getOperation().equals(Add.OPERATION))
                && !getOperation().equals(Divide.OPERATION);
    }
}
