package expression.generic.expression;

@FunctionalInterface
public interface TripleExpression<T extends Number> extends ToMiniString {
    T evaluate(T x, T y, T z);
}
