package markup;

public class Text implements Element {
    private final String value;

    public Text(String textValue) {
        value = textValue;
    }

    public String toString() {
        return value;
    }

    public void toMarkdown(StringBuilder builder) {
        builder.append(value);
    }

    public void toHtml(StringBuilder builder) {
        for (char c : value.toCharArray()) {
            switch (c) {
                case '<' -> builder.append("&lt;");
                case '>' -> builder.append("&gt;");
                case '&' -> builder.append("&amp;");
                case '"' -> builder.append("&quot;");
                default -> builder.append(c);
            }
        }
    }
}
