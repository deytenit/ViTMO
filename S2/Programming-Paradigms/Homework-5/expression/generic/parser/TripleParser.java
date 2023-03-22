package expression.generic.parser;

import expression.generic.expression.TripleExpression;
import expression.generic.expression.exceptions.ExpressionParsingException;

@FunctionalInterface
public interface TripleParser<T extends Number> {
    TripleExpression<T> parse(final String expression) throws ExpressionParsingException;
}
