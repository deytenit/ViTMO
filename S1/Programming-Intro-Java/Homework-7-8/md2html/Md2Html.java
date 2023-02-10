package md2html;

import markup.*;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class Md2Html {
    public static void main(String[] args) {

        String input;

        try {
            input = Files.readString(Paths.get(args[0]));
        } catch (IOException ioe) {
            System.out.println("Error while reading file: " + ioe.getMessage());
            return;
        }

        AbstractTokenTree.Token tree = new AbstractTokenTree(input).tokenize();

        Root root = parse(tree);

        StringBuilder out = new StringBuilder();

        root.toHtml(out);

        try {
            File output = new File(args[1]);
            output.createNewFile();
            Files.writeString(output.toPath(), out.toString());
        } catch (IOException ioe) {
            System.out.println("Error while writing to file:" + ioe.getMessage());
        }
    }

    private static Root parse(AbstractTokenTree.Token token) {
        List<Container> containers = new ArrayList<>();

        for (AbstractTokenTree.Token child : token) {
            containers.add(parseContainer(child));
        }

        return new Root(containers);
    }

    private static Container parseContainer(AbstractTokenTree.Token token) {
        List<Element> elements = new ArrayList<>();

        for (AbstractTokenTree.Token child : token) {
            elements.add(parseElement(child));
        }

        if (token.key().equals("p")) {
            return new Paragraph(elements);
        } else {
            return new Heading(elements, token.key().length());
        }
    }

    private static Element parseElement(AbstractTokenTree.Token token) {
        if (token.isEmpty()) {
            return new Text(token.key());
        }


        List<Element> elements = new ArrayList<>();

        for (AbstractTokenTree.Token child : token) {
            elements.add(parseElement(child));
        }

        if (token.key().equals("![")) {
            return new Image((Text) elements.get(0));
        }

        if (token.key().equals("**") || token.key().equals("__")) {
            return new Strong(elements);
        }

        if (token.key().equals("--")) {
            return new Strikeout(elements);
        }

        if (token.key().equals("*") || token.key().equals("_")) {
            return new Emphasis(elements);
        }

        return new Code(elements);
    }
}
