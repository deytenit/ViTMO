package expression.generic.expression.exceptions;

public class UnderflowException extends ExpressionEvaluationException {
    public UnderflowException(final String msg, final String operation, final String expression) {
        super(msg, operation, expression);
    }
}
