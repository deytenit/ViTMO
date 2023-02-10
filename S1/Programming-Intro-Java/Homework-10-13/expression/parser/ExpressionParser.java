package expression.parser;

import java.util.List;

import expression.*;


public class ExpressionParser implements TripleParser {

    static final List<String> operations = List.of(
            Add.OPERATION, Subtract.OPERATION, Multiply.OPERATION, Divide.OPERATION
    );
    static final List<String> directives = List.of(
            BitwiseSet.OPERATION, BitwiseClear.OPERATION, BitwiseCount.OPERATION,
            Log10.OPERATION, Pow10.OPERATION
    );
    static final List<Character> variables = List.of('x', 'y', 'z');

    String expression;
    int pos;

    @Override
    public TripleExpression parse(String expression) {
        System.err.println(expression);

        return universalParser(expression);
    }

    public UniversalExpression universalParser(String expression) {
        this.expression = expression;
        this.pos = 0;

        return parseLow();
    }

    private UniversalExpression parseLow() {
        UniversalExpression clause = parseMid();

        if (pos == expression.length()) {
            return clause;
        }

        while (pos != expression.length()) {
            String directive = getDirective();

            if (directive.equals(BitwiseSet.OPERATION)) {
                clause = new BitwiseSet(clause, parseMid());
            } else if (directive.equals(BitwiseClear.OPERATION)) {
                clause = new BitwiseClear(clause, parseMid());
            } else {
                throw new IllegalStateException(
                        "Not a valid directive: " + directive
                );
            }
        }

        return clause;
    }

    private UniversalExpression parseMid() {
        UniversalExpression term = parseTop();

        while (pos != expression.length()) {
            if (expression.charAt(pos) == '+') {
                ++pos;
                term = new Add(term, parseTop());
            }
            else if (expression.charAt(pos) == '-') {
                ++pos;
                term = new Subtract(term, parseTop());
            }
            else if (Character.isWhitespace(expression.charAt(pos))) {
                ++pos;
            }
            else {
                break;
            }
        }

        return term;
    }

    private UniversalExpression parseTop() {
        UniversalExpression internal = parseInternal();

        while (pos != expression.length()) {
            if (expression.charAt(pos) == '*') {
                ++pos;
                internal = new Multiply(internal, parseInternal());
            }
            else if (expression.charAt(pos) == '/') {
                ++pos;
                internal = new Divide(internal, parseInternal());
            }
            else if (Character.isWhitespace(expression.charAt(pos))) {
                ++pos;
            }
            else {
                break;
            }
        }

        return internal;
    }

    private UniversalExpression parseInternal() {
        skipWhitespaces();

        if (expression.charAt(pos) == '(') {
            ExpressionParser subParser = new ExpressionParser();
            return subParser.universalParser(getInnerBracketsContent());
        }

        if (Character.isDigit(expression.charAt(pos)) ||
                expression.charAt(pos) == '-' && Character.isDigit(expression.charAt(pos + 1))) {
            return new Const(getConstant());
        }

        if (variables.contains(expression.charAt(pos))) {
            return new Variable(getVariable());
        }

        if (expression.charAt(pos) == '-') {
            ++pos;
            return new Negate(parseInternal());
        }

        String directive = getDirective();

        if (directive.equals(Log10.OPERATION)) {
            return new Log10(parseInternal());
        }

        if (directive.equals(Pow10.OPERATION)) {
            return new Pow10(parseInternal());
        }

        if (directive.equals(BitwiseCount.OPERATION)) {
            return new BitwiseCount(parseInternal());
        }


        throw new IllegalStateException("Variable or constant cannot be parsed or undefined.");
    }

    private void skipWhitespaces() {
        while (Character.isWhitespace(expression.charAt(pos))) {
            ++pos;
        }
    }

    private String getInnerBracketsContent() {
        int bracketCounter = 0;
        StringBuilder inner = new StringBuilder();

        do {
            if (expression.charAt(pos) == ')') {
                --bracketCounter;
            }

            if (bracketCounter != 0) {
                inner.append(expression.charAt(pos));
            }

            if (expression.charAt(pos) == '(') {
                ++bracketCounter;
            }

            ++pos;
        } while (bracketCounter != 0 && pos < expression.length());

        if (bracketCounter != 0) {
            throw new IllegalStateException("Closing bracket was not found in: " + inner);
        }

        return inner.toString();
    }

    private int getConstant() {
        int result = 0;
        boolean negative = false;

        if (expression.charAt(pos) == '-') {
            negative = true;
            ++pos;
        }

        while (pos < expression.length() && Character.isDigit(expression.charAt(pos))) {
            result = result * 10 + Character.getNumericValue(expression.charAt(pos++));
        }

        if (pos != expression.length()
                && !Character.isWhitespace(expression.charAt(pos))
                && !operations.contains(expression.substring(pos, pos + 1)))  {
            throw new IllegalStateException("Not a valid constant: " + result + expression.charAt(pos));
        }

        return negative ? -result : result;
    }

    private String getVariable() {
        StringBuilder variable = new StringBuilder();

        while (pos < expression.length() && Character.isJavaIdentifierPart(expression.charAt(pos))) {
            variable.append(expression.charAt(pos++));
        }

        String res = variable.toString();

        if (res.length() != 1 || !variables.contains(res.charAt(0))){
            throw new IllegalStateException("Not a valid variable name: " + res);
        }

        return res;
    }

    private String getDirective() {
        StringBuilder identifier = new StringBuilder();

        while (pos < expression.length() && Character.isJavaIdentifierPart(expression.charAt(pos))) {
            identifier.append(expression.charAt(pos++));
        }

        String res = identifier.toString();

        if (!directives.contains(res)) {
            throw new IllegalStateException("Not a valid directive: " + res);
        }

        return res;
    }
}

