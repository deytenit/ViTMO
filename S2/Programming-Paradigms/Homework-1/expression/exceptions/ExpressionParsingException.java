package expression.exceptions;

public class ExpressionParsingException extends Exception {
    final int pos;
    final String expr;

    public ExpressionParsingException(final String msg, final int pos) {
        super("Ch. " + pos + ": " + msg);
        this.pos = pos;
        expr = null;
    }

    public ExpressionParsingException(final String msg, final int pos, final String expr) {
        super("Ch. " + pos + " -> " + expr + " : " + msg);
        this.pos = pos;
        this.expr = expr;
    }
}
