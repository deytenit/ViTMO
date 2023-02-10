package markup;

public interface HtmlElement extends VirtualDocument {
    void toHtml(StringBuilder builder);
}
