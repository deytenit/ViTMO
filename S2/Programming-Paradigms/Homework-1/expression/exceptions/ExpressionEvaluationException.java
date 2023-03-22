package expression.exceptions;

public class ExpressionEvaluationException extends ArithmeticException {
    final String expr;
    final String operation;
    public ExpressionEvaluationException(final String msg, final String operation, final String expr) {
        super(operation + " -> " + expr + " : " + msg);

        this.operation = operation;
        this.expr = expr;
    }
}
