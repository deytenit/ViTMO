package expression.generic.expression.exceptions;

public class OverflowException extends ExpressionEvaluationException {
    public OverflowException(final String msg, final String operation, final String expression) {
        super(msg, operation, expression);
    }
}
