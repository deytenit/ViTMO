package expression;

import java.util.Objects;

public class Variable implements UniversalExpression {
    private final String variable;

    public Variable(String variable) {
        this.variable = variable;
    }

    @Override
    public int getPriority() {
        return 0;
    }

    @Override
    public int evaluate(int x) {
        return x;
    }

    @Override
    public double evaluate(double x) {
        return x;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return switch (variable) {
            case "x" -> x;
            case "y" -> y;
            case "z" -> z;
            default -> throw new RuntimeException("Variable evaluation is not possible for " + variable);
        };
    }

    @Override
    public String toString() {
        return variable;
    }

    @Override
    public String toMiniString() {
        return variable;
    }

    @Override
    public int hashCode() {
        return variable.hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Variable variable1 = (Variable) o;
        return Objects.equals(variable, variable1.variable);
    }
}
