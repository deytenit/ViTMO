package expression;

public interface UniversalExpression extends Expression, DoubleExpression, TripleExpression {
    int getPriority();
}
