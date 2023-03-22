package expression.generic.expression;

import expression.generic.expression.modes.Mode;

public abstract class Expression<T extends Number> implements UniversalExpression<T> {

    final Mode<T> mode;

    public Expression(final Mode<T> mode) {
        this.mode = mode;
    }

    @Override
    public Mode<T> getMode() {
        return mode;
    }
}
