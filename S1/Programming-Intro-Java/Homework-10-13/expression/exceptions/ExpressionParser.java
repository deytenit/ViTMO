package expression.exceptions;

import java.util.List;

import expression.*;


public class ExpressionParser implements TripleParser {

    static final List<String> operations = List.of(
            CheckedAdd.OPERATION, CheckedSubtract.OPERATION, CheckedMultiply.OPERATION, CheckedDivide.OPERATION
    );
    static final List<String> directives = List.of(
            BitwiseSet.OPERATION, BitwiseClear.OPERATION, BitwiseCount.OPERATION,
            CheckedLog10.OPERATION, CheckedPow10.OPERATION
    );
    static final List<Character> variables = List.of('x', 'y', 'z');

    String expression;
    int pos;
    @Override
    public TripleExpression parse(String expression) throws ExpressionParsingException {
        //System.err.println(expression);

        return universalParser(expression);
    }

    public UniversalExpression universalParser(String expression) throws ExpressionParsingException {
        this.expression = expression;
        this.pos = 0;

        return parseLow();
    }

    private UniversalExpression parseLow() throws ExpressionParsingException {
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
            }
        }

        return clause;
    }

    private UniversalExpression parseMid() throws ExpressionParsingException {
        UniversalExpression term = parseTop();

        while (pos != expression.length()) {
            if (expression.charAt(pos) == '+') {
                ++pos;
                term = new CheckedAdd(term, parseTop());
            }
            else if (expression.charAt(pos) == '-') {
                ++pos;
                term = new CheckedSubtract(term, parseTop());
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

    private UniversalExpression parseTop() throws ExpressionParsingException {
        UniversalExpression internal = parseInternal();

        while (pos != expression.length()) {
            if (expression.charAt(pos) == '*') {
                ++pos;
                internal = new CheckedMultiply(internal, parseInternal());
            }
            else if (expression.charAt(pos) == '/') {
                ++pos;
                internal = new CheckedDivide(internal, parseInternal());
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

    private UniversalExpression parseInternal() throws ExpressionParsingException {
        skipWhitespaces();

        if (pos == expression.length()) {
            throw new ExpressionParsingException("Expression end was unexpected.", pos, "...?");
        }

        if (expression.charAt(pos) == '(') {
            ExpressionParser subParser = new ExpressionParser();
            return subParser.universalParser(getInnerBracketsContent());
        }

        if (Character.isDigit(expression.charAt(pos))
                || pos + 1 != expression.length()
                && expression.charAt(pos) == '-' && Character.isDigit(expression.charAt(pos + 1))) {
            return new Const(getConstant());
        }

        if (variables.contains(expression.charAt(pos))) {
            return new Variable(getVariable());
        }

        if (pos + 1 != expression.length() && expression.charAt(pos) == '-') {
            ++pos;
            return new CheckedNegate(parseInternal());
        }

        String directive = getDirective();

        if (directive.equals(CheckedLog10.OPERATION)) {
            return new CheckedLog10(parseInternal());
        }

        if (directive.equals(CheckedPow10.OPERATION)) {
            return new CheckedPow10(parseInternal());
        }

        if (directive.equals(BitwiseCount.OPERATION)) {
            return new BitwiseCount(parseInternal());
        }

        throw new ExpressionParsingException("Identifier lost.", pos - directive.length(), "? " + directive);
    }

    private void skipWhitespaces() {
        while (pos < expression.length() && Character.isWhitespace(expression.charAt(pos))) {
            ++pos;
        }
    }

    private String getInnerBracketsContent() throws ExpressionParsingException {
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
            throw new ExpressionParsingException("Closing bracket lost.", pos, "(" + inner + "...");
        }

        return inner.toString();
    }

    private int getConstant() throws ExpressionParsingException {
        int result = 0;
        boolean negative = false;

        if (expression.charAt(pos) == '-') {
            negative = true;
            ++pos;
        }

        while (pos < expression.length() && Character.isDigit(expression.charAt(pos))) {
            final int digit = Character.getNumericValue(expression.charAt(pos++));
            if (!negative && (result > Integer.MAX_VALUE / 10 || result * 10 > Integer.MAX_VALUE - digit)) {
                throw new ExpressionParsingException("Overflowed constant.", pos, result + String.valueOf(digit) + "?");
            }

            if (negative && (-result < Integer.MIN_VALUE / 10 || -result * 10 < Integer.MIN_VALUE + digit)) {
                throw new ExpressionParsingException("Underflowed constant.", pos, -result + String.valueOf(digit) + "?");
            }

            result = result * 10 + digit;
        }

        if (pos != expression.length() && expression.charAt(pos) == ')') {
            throw new ExpressionParsingException("Unopened closing bracket.", pos, "?...)");
        }

        if (pos != expression.length()
                && !Character.isWhitespace(expression.charAt(pos))
                && !operations.contains(expression.substring(pos, pos + 1)))  {
            throw new ExpressionParsingException(
                    "Not a valid constant.",
                    pos,
                    result + expression.substring(pos, pos + 1)
            );
        }

        return negative ? -result : result;
    }

    private String getVariable() throws ExpressionParsingException {
        StringBuilder variable = new StringBuilder();

        while (pos < expression.length() && Character.isJavaIdentifierPart(expression.charAt(pos))) {
            variable.append(expression.charAt(pos++));
        }

        String res = variable.toString();

        if (res.length() != 1 || !variables.contains(res.charAt(0))) {
            throw new ExpressionParsingException("Not a valid variable name.", pos, res);
        }

        if (pos != expression.length() && expression.charAt(pos) == ')') {
            throw new ExpressionParsingException("Unopened closing bracket.", pos, "?...)");
        }

        return res;
    }

    private String getDirective() throws ExpressionParsingException {
        if (pos == expression.length()) {
            throw new ExpressionParsingException("Expression end was unexpected.", pos, "...?");
        }

        if (expression.charAt(pos) == ')') {
            throw new ExpressionParsingException("Unopened closing bracket.", pos, "?...)");
        }

        StringBuilder identifier = new StringBuilder();

        while (pos < expression.length() && Character.isJavaIdentifierPart(expression.charAt(pos))) {
            identifier.append(expression.charAt(pos++));
        }

        String res = identifier.toString();

        if (res.isEmpty()) {
            throw new ExpressionParsingException("Expression is malformed.", pos);
        }

        if (!directives.contains(res)) {
            throw new ExpressionParsingException("Not a valid directive.", pos - res.length(), res);
        }

        return res;
    }
}

