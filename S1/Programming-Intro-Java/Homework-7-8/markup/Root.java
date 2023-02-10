package markup;

import java.util.List;

public class Root implements HtmlElement {
    private final List<Container> containers;

    public Root(List<Container> initContainerList) {
        containers = initContainerList;
    }

    @Override
    public void toHtml(StringBuilder builder) {
        for (Container container: containers) {
            container.toHtml(builder);
        }
    }
}

