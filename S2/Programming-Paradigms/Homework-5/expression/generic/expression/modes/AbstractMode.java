package expression.generic.expression.modes;

import expression.generic.parser.ExpressionParser;

public abstract class AbstractMode<T extends Number> implements Mode<T> {
    @Override
    public ExpressionParser<T> parser() {
        return new ExpressionParser<>(this);
    }
}
