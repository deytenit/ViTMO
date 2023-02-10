package markup;

import java.util.ArrayList;
import java.util.List;

public class Image implements Element {

    private final String alt;
    private final String src;

    public Image(Text initElement) {
        String tmp = initElement.toString();

        alt = tmp.substring(0, tmp.indexOf("]("));
        src = tmp.substring(tmp.indexOf("](") + 2);
    }

    @Override
    public void toHtml(StringBuilder builder) {
        builder.append("<img alt='");
        builder.append(alt);
        builder.append("' src='");
        builder.append(src);
        builder.append("'>");
    }

    @Override
    public void toMarkdown(StringBuilder builder) {
        builder.append("![");
        builder.append(alt);
        builder.append("](");
        builder.append(src);
        builder.append(')');
    }
}
