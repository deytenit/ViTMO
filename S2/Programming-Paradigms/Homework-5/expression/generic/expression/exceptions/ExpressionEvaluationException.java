package expression.generic.expression.exceptions;

public class ExpressionEvaluationException extends ArithmeticException {
    final String expression;
    final String operation;
    public ExpressionEvaluationException(final String msg, final String operation, final String expression) {
        super(operation + " -> " + expression + " : " + msg);

        this.operation = operation;
        this.expression = expression;
    }
}
