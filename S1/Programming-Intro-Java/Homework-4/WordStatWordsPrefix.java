import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.Arrays;

public class WordStatWordsPrefix {
    public static void main(String[] args) {
        String[] words = new String[0];
        
        try {
            words = splitFile(args[0]);
        }
        catch (IOException e) {
            System.out.println("Error while reading file: " + e.getMessage());
            return;
        }

        Arrays.sort(words);

        int[] counts = new int[words.length];

        int i = 0;
        while (i < words.length) {
            int cnt = 1;

            while (i + cnt < words.length && words[i].equals(words[i + cnt])) {
                counts[i + cnt] = -1;
                cnt++;
            }

            counts[i] = cnt;

            i += cnt;
        }

        try {
            printStatFile(args[1], words, counts);
        }
        catch (IOException e) {
            System.out.println("Error while writing file: " + e.getMessage());
            return;
        }
    }
    
    private static String[] splitFile(String path) throws IOException {
        int resultSize = 0;
        String[] result = new String[1];

        BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(path), "UTF8"));
        
        try {
            int wordSize = 0;
            char[] word = new char[3];

            int read;

            while ((read = reader.read()) >= 0) {
                char curCh = (char) read;
                boolean wordReady = !reader.ready();

                if (Character.isLetter(curCh) || curCh == '\'' || Character.getType(curCh) == Character.DASH_PUNCTUATION) {
                    if (wordSize < 3) {
                        word[wordSize++] = Character.toLowerCase(curCh);
                    }
                }
                else {
                    wordReady = true;
                }
                
                if (wordReady && wordSize > 0) {
                    if (resultSize == result.length) {
                        result = Arrays.copyOf(result, result.length * 2);
                    }

                    result[resultSize++] = String.valueOf(word, 0, wordSize);

                    wordSize = 0;
                }
            }
        }
        finally {
            reader.close();
        }

        return Arrays.copyOf(result, resultSize);
    }

    private static void printStatFile(String path, String[] words, int[] counts) throws IOException {
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path), "UTF8"));

        try {
            for (int i = 0; i < words.length; i++) {
                if (counts[i] != -1) {
                    writer.write(words[i] + " " + counts[i]);
                    writer.newLine();
                }
            }
        }
        finally {
            writer.close();
        }
    }

}
