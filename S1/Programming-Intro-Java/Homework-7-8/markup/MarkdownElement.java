package markup;

public interface MarkdownElement extends VirtualDocument {
    void toMarkdown(StringBuilder builder);
}
