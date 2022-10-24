import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class WordStatInput {
    public static void main(String[] args) {
        String[] words = new String[1];
        int wordsSize = 0;
        
        try {
            MyScanner wsc = new MyScanner(new File(args[0]));
            wsc.setPredicate(c -> !Character.isLetter(c) && c != '\'' && Character.getType(c) != Character.DASH_PUNCTUATION);
            try {
                while (wsc.hasNext()) {
                    if (wordsSize == words.length) {
                        words = Arrays.copyOf(words, wordsSize * 2);
                    }
                    words[wordsSize++] = wsc.next().toLowerCase();
                }
            }
            finally {
                wsc.close();
            }
        }
        catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
            return;
        }

        words = Arrays.copyOf(words, wordsSize);

        int[] idxs = new int[words.length];
        for (int i = 0; i < words.length; i++) {
            idxs[i] = i;
        }
                
        mergeSort(words, idxs, 0, words.length);

        int[] counts = new int[words.length];
        int[] idxsRev = new int[words.length];

        for (int i = 0; i < words.length; i++) {
            idxsRev[idxs[i]] = i;
        }

        for (int i = 0; i < words.length;) {
            int cnt = 1;

            String word = words[i];

            while (i + cnt < words.length && word.equals(words[i + cnt])) {
                counts[i + cnt] = -1;
                cnt++;
            }

            counts[i] = cnt;

            i += cnt;
        }

        try {
            printStatFile(args[1], words, counts, idxsRev);
        }
        catch (IOException e) {
            System.out.println("Error writing file: " + e.getMessage());
        }
    }

    private static void merge(String[] strs, int[] idxs, int begin, int mid, int end) {
        int l = begin;
        int r = mid;

        String[] mergedStrs = new String[end - begin];
        int[] mergedIdxs = new int[end - begin];

        while (l < mid || r < end) {
            if (r == end || (l < mid && strs[l].compareTo(strs[r]) <= 0)) {
                mergedStrs[l - begin + r - mid] = strs[l];
                mergedIdxs[l - begin + r - mid] = idxs[l];
                l++;
            } else if (l == mid || (r < end && strs[r].compareTo(strs[l]) < 0)) {
                mergedStrs[l - begin + r - mid] = strs[r];
                mergedIdxs[l - begin + r - mid] = idxs[r];
                r++;
            }
        }

        for (int i = begin; i < end; i++) {
            strs[i] = mergedStrs[i - begin];
            idxs[i] = mergedIdxs[i - begin]; 
        }
    }

    private static void mergeSort(String[] strs, int[] idxs, int begin, int end) {
        if (end - begin == 1) {
            return;
        }

        int mid = (begin + end) / 2;

        mergeSort(strs, idxs, begin, mid);
        mergeSort(strs, idxs, mid, end);

        merge(strs, idxs, begin, mid, end);
    }

    private static void printStatFile(String path, String[] words, int[] counts, int[] idxs) throws IOException {
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path), StandardCharsets.UTF_8));

        try {
            for (int i = 0; i < words.length; i++) {
                if (counts[idxs[i]] != -1) {
                    writer.write(words[idxs[i]] + " " + counts[idxs[i]]);
                    writer.newLine();
                }
            }
        }
        finally {
            writer.close();
        }
    }

}
