package markup;

import java.util.List;

public class Strikeout extends MarkupBase {
    public Strikeout(List<Element> list) {
        super(list);
    }

    public void toMarkdown(StringBuilder builder) {
        toMarkdown(builder, "~", "~");
    }

    public void toHtml(StringBuilder stringBuilder) {
        toHtml(stringBuilder, "<s>", "</s>");
    }
}
