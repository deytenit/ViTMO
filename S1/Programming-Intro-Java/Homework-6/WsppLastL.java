import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class WsppLastL {
    public static void main(String[] args) {
        Map<String, IntList> words = new LinkedHashMap<>(); // :NOTE: use interface

        try {
            MyScanner lsc = new MyScanner(new File(args[0]));
            while (lsc.hasNextLine()) {
                MyScanner wsc = new MyScanner(lsc.nextLine());
                wsc.setPredicate(c -> !Character.isLetter(c) && c != '\''
                    && Character.getType(c) != Character.DASH_PUNCTUATION);


                Map<String, IntList> temp = new LinkedHashMap<>(); // :NOTE: use interface
                int tempCount = 0;

                try {
                    while (wsc.hasNext()) {
                        String word = wsc.next().toLowerCase();

                        if (temp.containsKey(word)) {
                            temp.get(word).set(0, temp.get(word).get(0) + 1);
                            temp.get(word).set(1, tempCount++);
                        } else {
                            IntList list = new IntList();
                            list.add(1);
                            list.add(tempCount++);
                            temp.put(word, list);
                        }
                    }
                } finally {
                    wsc.close();
                }

                for (Map.Entry<String, IntList> item : temp.entrySet()) {
                    if (words.containsKey(item.getKey())) {
                        IntList value = words.get(item.getKey());
                        value.set(0, value.get(0) + item.getValue().get(0));
                        value.add(item.getValue().get(1));
                    } else {
                        IntList value = new IntList();
                        value.add(item.getValue().get(0));
                        value.add(item.getValue().get(1));

                        words.put(item.getKey(), value);

                    }
                }
            }

            lsc.close();
        } catch (FileNotFoundException e) {
            System.out.println("Error reading file: " + e.getMessage());
            return;
        }

        try {
            printStatFile(args[1], words);
        } catch (IOException e) {
            System.out.println("Error writing file: " + e.getMessage());
        }
    }

    private static void printStatFile(String path, Map<String, IntList> words) throws
                                                                                   IOException { // :NOTE: use interface instead of implementations
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
            new FileOutputStream(path),
            StandardCharsets.UTF_8
        ));

        try {
            for (Map.Entry<String, IntList> item : words.entrySet()) {
                writer.write(item.getKey() + " " + item.getValue().get(0));
                for (int i = 1; i < item.getValue().size(); i++) {
                    writer.write(" ");
                    writer.write(String.valueOf(item.getValue().get(i) + 1));
                }
                writer.newLine();
            }
        } finally {
            writer.close();
        }
    }

}
