package markup;

import java.util.List;

public abstract class ContainerBase implements Container, MarkdownElement {
    private final List<Element> children;

    protected ContainerBase(List<Element> listOfElements) {
        children = listOfElements;
    }

    void toHtml(StringBuilder builder, String beginMark, String endMark) {
        builder.append(beginMark);

        for (Element element : children) {
            element.toHtml(builder);
        }

        builder.append(endMark);
        builder.append(System.lineSeparator());
    }

    void toMarkdown(StringBuilder builder, String beginMark, String endMark) {
        builder.append(beginMark);

        for (Element element : children) {
            element.toMarkdown(builder);
        }

        builder.append(endMark);
        builder.append(System.lineSeparator());
    }
}
