package expression.exceptions;

import expression.Divide;
import expression.UniversalExpression;

public class CheckedDivide extends Divide {
    public CheckedDivide(UniversalExpression lValue, UniversalExpression rValue) {
        super(lValue, rValue);
    }

    @Override
    public int calculate(int a, int b) {
        if (b == 0) {
            throw new ExpressionEvaluationException("Division by zero.", getOperation(), toMiniString());
        }

        if (b == -1 && a == Integer.MIN_VALUE) {
            throw new ExpressionEvaluationException("Overflow.", getOperation(), toMiniString());
        }

        return super.calculate(a, b);
    }
}
