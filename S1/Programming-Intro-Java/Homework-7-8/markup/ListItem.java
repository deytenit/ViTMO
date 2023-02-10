package markup;

import java.util.List;

public class ListItem implements HtmlElement {
    private final List<Container> containers;

    public ListItem(List<Container> list) {
        containers = list;
    }

    @Override
    public void toHtml(StringBuilder stringBuilder) {
        stringBuilder.append("<li>");
        for (Container container: containers) {
            container.toHtml(stringBuilder);
        }
        stringBuilder.append("</li>");
    }
}
