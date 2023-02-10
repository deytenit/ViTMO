package markup;

import java.util.List;

public class Code extends MarkupBase {
    public Code(List<Element> initElementList) {
        super(initElementList);
    }

    @Override
    public void toHtml(StringBuilder builder) {
        toHtml(builder, "<code>", "</code>");
    }

    @Override
    public void toMarkdown(StringBuilder builder) {
        toMarkdown(builder, "`", "`");
    }
}
