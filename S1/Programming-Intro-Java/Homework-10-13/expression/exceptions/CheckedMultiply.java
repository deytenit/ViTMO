package expression.exceptions;

import expression.Multiply;
import expression.UniversalExpression;

public class CheckedMultiply extends Multiply {
    public CheckedMultiply(UniversalExpression lValue, UniversalExpression rValue) {
        super(lValue, rValue);
    }

    @Override
    public int calculate(int a, int b) {
        int res = super.calculate(a, b);

        if (a != 0 && res / a != b
                || (a > 0 && b > 0 || a < 0 && b < 0) && res < 0
                || (a < 0 && b > 0 || a > 0 && b < 0) && res > 0) {
            if (a > 0 && b > 0 || a < 0 && b < 0) {
                throw new ExpressionEvaluationException("Overflow.", getOperation(), toMiniString());
            }
            else {
                throw new ExpressionEvaluationException("Underflow.", getOperation(), toMiniString());
            }
        }

        return res;
    }
}
