package expression.exceptions;

import expression.Pow10;
import expression.UniversalExpression;

public class CheckedPow10 extends Pow10 {
    final int INTEGER_POW_LIMIT = 9;
    public CheckedPow10(UniversalExpression value) {
        super(value);
    }

    @Override
    protected int calculate(int a) {
        if (a < 0) {
            throw new ExpressionEvaluationException("Power cannot be negative.", getOperation(), toMiniString());
        }

        if (a > INTEGER_POW_LIMIT) {
            throw new ExpressionEvaluationException("Overflow.", getOperation(), toMiniString());
        }

        return super.calculate(a);
    }
}
