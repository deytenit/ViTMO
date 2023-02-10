package md2html;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class AbstractTokenTree {
    public static class Token extends ArrayList<Token> {
        private final String key;

        public Token(String initKey) {
            super();
            key = initKey;
        }

        public String key() {
            return key;
        }
    }

    private final Token root;

    public AbstractTokenTree(String data) {
        root = new Token(".");

        String[] paragraphs = data.split("^(\\R+)|(\\R{2,})|(\\R+)$");

        for (String paragraph : paragraphs) {
            if (!paragraph.isEmpty()) {
                root.add(tokenize(paragraph, 1));
            }
        }
    }

    public Token tokenize() {
        return root;
    }

    private static boolean isDelimiter(char delim) {
        return delim == '*' || delim == '_' || delim == '-' || delim == '`'
            || delim == '!' || delim == '[' || delim == ')';
    }

    private static boolean isKey(String key) {
        return key.equals("**") || key.equals("__") || key.equals("--") || key.equals("*") || key.equals("_")
            || key.equals("`")
            || key.equals("![") || key.equals(")");
    }

    private static boolean isKey(String key, String closing) {
        return key.equals("![") && closing.equals(")") || isKey(key) && key.equals(closing);
    }

    private static boolean isIgnore(String key) {
        return key.equals("![");
    }

    private static Pair<String, String> parseKey(String token, int level) {
        if (level == 1) {
            Matcher matcher = Pattern.compile("^(#+ +)").matcher(token);

            if (matcher.find()) {
                return new Pair<>(matcher.group().trim(), matcher.replaceFirst(""));
            } else {
                return new Pair<>("p", token);
            }
        }

        if (token.length() > 2 && isKey(token.substring(0, 2), token.substring(token.length() - 1))) {
            return new Pair<>(token.substring(0, 2), token.substring(2, token.length() - 1));
        }

        if (token.length() > 3
            && isKey(token.substring(0, 2), token.substring(token.length() - 2))) {
            return new Pair<>(token.substring(0, 2), token.substring(2, token.length() - 2));
        }

        if (token.length() > 1 && token.charAt(0) == token.charAt(token.length() - 1) && isDelimiter(token.charAt(0))) {
            return new Pair<>(token.substring(0, 1), token.substring(1, token.length() - 1));
        }

        return new Pair<>(token, "");
    }

    private static Token tokenize(String data, int level) {
        Pair<String, String> metadata = parseKey(data, level);

        Token result = new Token(metadata.first);

        if (metadata.second.isEmpty()) {
            return result;
        } else {
            data = metadata.second;
        }

        List<String> tokens = isIgnore(metadata.first) ? Collections.singletonList(data) : lexicalAnalysis(data);

        StringBuilder token = new StringBuilder();

        for (int i = 0; i < tokens.size(); ++i) {
            if (isKey(tokens.get(i))) {
                String key = tokens.get(i);

                do {
                    token.append(tokens.get(i++));
                } while (i < tokens.size() && !isKey(key, tokens.get(i)));
            }

            if (i < tokens.size()) {
                token.append(tokens.get(i));
            }

            result.add(tokenize(token.toString(), level + 1));
            token.setLength(0);
        }

        return result;
    }

    private static List<String> lexicalAnalysis(String data) {
        List<String> tokens = new ArrayList<>();

        StringBuilder token = new StringBuilder();
        boolean ifEscaped = false;
        boolean ifKey = false;

        for (char chr : data.toCharArray()) {
            if (ifEscaped) {
                token.append(chr);
                ifEscaped = false;
                continue;
            }

            if (chr == '\\') {
                ifEscaped = true;
                continue;
            }

            if (isDelimiter(chr)) {
                if (ifKey && isKey(String.valueOf(new char[]{token.charAt(token.length() - 1), chr}))) {
                    token.append(chr);
                } else {
                    if (!token.isEmpty()) {
                        tokens.add(token.toString());
                        token.setLength(0);
                    }

                    token.append(chr);

                    ifKey = true;
                }
            } else if (ifKey) {
                if (!token.isEmpty()) {
                    tokens.add(token.toString());
                    token.setLength(0);
                }

                token.append(chr);

                ifKey = false;
            } else {
                token.append(chr);
            }
        }

        if (!token.isEmpty()) {
            tokens.add(token.toString());
            token.setLength(0);
        }

        return tokens;
    }

    private static class Pair<T, U> {
        public T first;
        public U second;

        public Pair(T first, U second) {
            this.first = first;
            this.second = second;
        }
    }
}
