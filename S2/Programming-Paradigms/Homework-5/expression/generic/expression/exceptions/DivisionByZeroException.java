package expression.generic.expression.exceptions;

public class DivisionByZeroException extends ExpressionEvaluationException {
    public DivisionByZeroException(final String msg, final String operation, final String expression) {
        super(msg, operation, expression);
    }
}
