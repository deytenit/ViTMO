package expression.generic.expression;

import expression.generic.expression.modes.Mode;

public interface UniversalExpression<T extends Number> extends TripleExpression<T> {
    int getPriority();
    Mode<T> getMode();
}
