package markup;

import java.util.List;

public class UnorderedList extends ListBase {
    public UnorderedList(List<ListItem> list) {
        super(list);
    }

    public void toHtml(StringBuilder stringBuilder) {
        toHtml(stringBuilder, "<ul>", "</ul>");
    }
}
