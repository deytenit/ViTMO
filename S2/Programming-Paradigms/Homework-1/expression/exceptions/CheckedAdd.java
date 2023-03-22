package expression.exceptions;

import expression.Add;
import expression.UniversalExpression;

public class CheckedAdd extends Add {
    public CheckedAdd(UniversalExpression lValue, UniversalExpression rValue) {
        super(lValue, rValue);
    }

    @Override
    public int calculate(int a, int b) {
        if (b > 0 && a > Integer.MAX_VALUE - b) {
            throw new ExpressionEvaluationException("Overflow.", getOperation(), toMiniString());
        }

        if (b < 0 && a < Integer.MIN_VALUE - b) {
            throw new ExpressionEvaluationException("Underflow.", getOperation(), toMiniString());
        }

        return super.calculate(a, b);
    }
}
