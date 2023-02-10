package markup;

import java.util.List;

public abstract class ListBase implements Container {
    private final List<ListItem> items;

    protected ListBase(List<ListItem> list) {
        items = list;
    }

    protected void toHtml(StringBuilder stringBuilder, String beginMarker, String endMarker) {
        stringBuilder.append(beginMarker);
        for (ListItem item: items) {
            item.toHtml(stringBuilder);
        }
        stringBuilder.append(endMarker);
    }
}
