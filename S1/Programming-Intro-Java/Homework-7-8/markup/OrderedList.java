package markup;

import java.util.List;

public class OrderedList extends ListBase {
    public OrderedList(List<ListItem> list) {
        super(list);
    }

    public void toHtml(StringBuilder stringBuilder) {
        toHtml(stringBuilder, "<ol>", "</ol>");
    }
}
