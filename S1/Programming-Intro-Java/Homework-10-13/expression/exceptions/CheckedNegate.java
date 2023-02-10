package expression.exceptions;

import expression.Negate;
import expression.UniversalExpression;

public class CheckedNegate extends Negate {
    public CheckedNegate(UniversalExpression value) {
        super(value);
    }

    @Override
    public int calculate(int a) {
        if (a == Integer.MIN_VALUE) {
            throw new ExpressionEvaluationException("Overflow", getOperation(), toMiniString());
        }

        return super.calculate(a);
    }
}
