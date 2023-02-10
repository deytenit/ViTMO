package markup;

import java.util.List;

public class Paragraph extends ContainerBase {
    public Paragraph(List<Element> listOfElements) {
        super(listOfElements);
    }

    @Override
    public void toMarkdown(StringBuilder builder) {
        toMarkdown(builder, "", "");
    }

    @Override
    public void toHtml(StringBuilder builder) {
        toHtml(builder, "<p>", "</p>");
    }
}
