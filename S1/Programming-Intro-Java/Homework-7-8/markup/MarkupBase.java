package markup;

import java.util.List;

public abstract class MarkupBase implements Element {
    private final List<Element> children;

    public MarkupBase(List<Element> listOfElements) {
        children = listOfElements;
    }

    protected void toMarkdown(StringBuilder stringBuilder, String beginMarker, String endMarker) {
        stringBuilder.append(beginMarker);
        for (Element markup: children) {
            markup.toMarkdown(stringBuilder);
        }
        stringBuilder.append(endMarker);
    }

    protected void toHtml(StringBuilder stringBuilder, String beginMarker, String endMarker) {
        stringBuilder.append(beginMarker);
        for (Element markup: children) {
            markup.toHtml(stringBuilder);
        }
        stringBuilder.append(endMarker);
    }
}
