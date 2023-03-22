package expression.generic.parser;

import expression.generic.expression.*;
import expression.generic.expression.exceptions.ExpressionParsingException;
import expression.generic.expression.modes.Mode;

import java.util.List;
import java.util.function.Predicate;

public class ExpressionParser<T extends Number> implements TripleParser<T> {

    private static final List<Character> variables = List.of('x', 'y', 'z');

    private final Mode<T> mode;
    private String expression;
    private int pos;
    private int parenthesis;

    public ExpressionParser(final Mode<T> mode) {
        this.mode = mode;
    }

    @Override
    public TripleExpression<T> parse(final String expression) throws ExpressionParsingException {
        this.expression = expression;

        return universalParser();
    }

    private UniversalExpression<T> universalParser() throws ExpressionParsingException {
        this.pos = 0;
        this.parenthesis = 0;

        UniversalExpression<T> result = parseLow();

        if (parenthesis > 0) {
            throw new ExpressionParsingException("Missed closing parenthesis.", pos, "(...?");
        }

        return result;
    }

    private UniversalExpression<T> parseLow() throws ExpressionParsingException {
        return parseMid();
    }

    private UniversalExpression<T> parseMid() throws ExpressionParsingException {
        UniversalExpression<T> term = parseTop();

        while (true) {
            skipWhitespaces();
            if (nextWillBe("+")) {
                term = new Add<>(term, parseTop());
            } else if (nextWillBe("-")) {
                term = new Subtract<>(term, parseTop());
            } else {
                break;
            }
        }

        return term;
    }

    private UniversalExpression<T> parseTop() throws ExpressionParsingException {
        UniversalExpression<T> internal = parseInternal();

        while (true) {
            skipWhitespaces();
            if (nextWillBe("*")) {
                internal = new Multiply<>(internal, parseInternal());
            } else if (nextWillBe("/")) {
                internal = new Divide<>(internal, parseInternal());
            } else if (nextWillBe("mod")) {
                internal = new Mod<>(internal, parseInternal());
            } else {
                break;
            }
        }

        return internal;
    }

    private UniversalExpression<T> parseInternal() throws ExpressionParsingException {
        skipWhitespaces();

        if (nextWillBe("-")) {
            if (Character.isDigit(expression.charAt(pos))) {
                return parseConstant(false);
            }

            return new Negate<>(parseInternal());
        }

        if (nextWillBe("abs")) {
            return new Abs<>(parseInternal());
        }

        if (nextWillBe("square")) {
            return new Square<>(parseInternal());
        }

        if (nextWillBe("(")) {
            ++parenthesis;

            skipWhitespaces();

            UniversalExpression<T> internal = parseLow();

            skipWhitespaces();

            if (!nextWillBe(")")) {
                throw new ExpressionParsingException("Missed closing parenthesis.", pos, "(...?");
            }

            --parenthesis;

            return internal;
        }

        if (Character.isDigit(expression.charAt(pos))) {
            return parseConstant(true);
        }

        return parseVariable();
    }

    private Const<T> parseConstant(final boolean positive) throws ExpressionParsingException {
        final StringBuilder constant = new StringBuilder();

        if (!positive) {
            constant.append('-');
        }

        while (isPos(Character::isDigit)) {
            constant.append(expression.charAt(pos++));
        }

        if (constant.isEmpty()) {
            throw new ExpressionParsingException("Unexpected token.", pos);
        }

        return new Const<>(constant.toString(), mode);
    }

    private Variable<T> parseVariable() throws ExpressionParsingException {
        final StringBuilder variable = new StringBuilder();

        while (isPos(Character::isJavaIdentifierPart)) {
            variable.append(expression.charAt(pos++));
        }

        if (variable.length() != 1 || !variables.contains(variable.charAt(0))) {
            throw new ExpressionParsingException("Not a valid variable name.", pos, variable.toString());
        }

        return new Variable<>(variable.toString(), mode);
    }

    private void skipWhitespaces() {
        while (isPos(Character::isWhitespace)) {
            ++pos;
        }
    }

    private boolean isPos(final Predicate<Character> predicate) {
        return pos < expression.length() && predicate.test(expression.charAt(pos));
    }

    private boolean nextWillBe(final String token) {
        final int begin = pos;

        for (final char c : token.toCharArray()) {
            if (pos == expression.length() || c != expression.charAt(pos++)) {
                pos = begin;
                return false;
            }
        }

        return true;
    }
}
