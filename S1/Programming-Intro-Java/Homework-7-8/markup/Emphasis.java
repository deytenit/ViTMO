package markup;

import java.util.List;

public class Emphasis extends MarkupBase {
    public Emphasis(List<Element> list) {
        super(list);
    }

    public void toMarkdown(StringBuilder builder) {
        toMarkdown(builder, "*", "*");
    }

    public void toHtml(StringBuilder stringBuilder) {
        toHtml(stringBuilder, "<em>", "</em>");
    }
}
