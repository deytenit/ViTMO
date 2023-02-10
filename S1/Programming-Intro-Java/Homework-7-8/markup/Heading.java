package markup;

import java.util.List;

public class Heading extends ContainerBase {
    int rank;

    public Heading(List<Element> listOfElements, int headingRank) {
        super(listOfElements);

        rank = headingRank;
    }

    @Override
    public void toMarkdown(StringBuilder builder) {
        toMarkdown(builder, "#".repeat(rank) + " ", "");
    }

    @Override
    public void toHtml(StringBuilder builder) {
        super.toHtml(builder, "<h" + rank + ">", "</h" + rank + ">");
    }
}
