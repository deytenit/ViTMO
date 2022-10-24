import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class Wspp {
    public static void main(String[] args) {
        Map<String, IntList> words = new LinkedHashMap<>();
        int entryCount = 0;

        try {
            MyScanner wsc = new MyScanner(new File(args[0]));
            wsc.setPredicate(c -> !Character.isLetter(c) && c != '\'' && Character.getType(c) != Character.DASH_PUNCTUATION);

            try {
                while (wsc.hasNext()) {
                    String word = wsc.next().toLowerCase();
                    if (words.containsKey(word)) {
                        words.get(word).add(entryCount++);
                    } else {
                        words.put(word, new IntList());
                        words.get(word).add(entryCount++);
                    }
                }
            }
            finally {
                wsc.close();
            }
        }
        catch (FileNotFoundException e) {
            System.out.println("Error reading file: " + e.getMessage());
            return;
        }

        try {
            printStatFile(args[1], words);
        }
        catch (IOException e) {
            System.out.println("Error writing file: " + e.getMessage());
        }
    }

    private static void printStatFile(String path, HashMap<String, IntList> words) throws IOException {
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path), StandardCharsets.UTF_8));

        try {
            for (Map.Entry<String, IntList> item: words.entrySet()) {
                writer.write(item.getKey() + " " + item.getValue().size());
                for (int i = 0; i < item.getValue().size(); i++) {
                    writer.write(" " + (item.getValue().get(i) + 1));
                }
                writer.newLine();
            }
        }
        finally {
            writer.close();
        }
    }

}
