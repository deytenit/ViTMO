package markup;

import java.util.List;

public class Strong extends MarkupBase {
    public Strong(List<Element> list) {
        super(list);
    }

    public void toMarkdown(StringBuilder builder) {
        toMarkdown(builder, "__", "__");
    }

    public void toHtml(StringBuilder stringBuilder) {
        toHtml(stringBuilder, "<strong>", "</strong>");
    }
}
