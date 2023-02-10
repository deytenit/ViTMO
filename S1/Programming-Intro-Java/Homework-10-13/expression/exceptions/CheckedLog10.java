package expression.exceptions;

import expression.Log10;
import expression.UniversalExpression;

public class CheckedLog10 extends Log10 {
    public CheckedLog10(UniversalExpression value) {
        super(value);
    }

    @Override
    protected int calculate(int a) {
        if (a <= 0) {
            throw new ExpressionEvaluationException("Argument must be positive.", getOperation(), toMiniString());
        }

        return super.calculate(a);
    }
}
