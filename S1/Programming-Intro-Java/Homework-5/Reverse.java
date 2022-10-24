import java.util.Arrays;

public class Reverse {
    public static void main(String[] args) {
        MyScanner lsc = new MyScanner(System.in);
        
        int[][] data = new int[1_000_000][];
        int[] temp = new int[1_000_000];

        int dataSize = 0;

        while (lsc.hasNextLine()) {
            MyScanner nsc = new MyScanner(lsc.nextLine());

            int tempSize = 0;

            while (nsc.hasNextInt()) {
                temp[tempSize++] = nsc.nextInt();
            }

            nsc.close();

            data[dataSize++] = Arrays.copyOf(temp, tempSize);
        }

        for (int i = dataSize - 1; i >= 0; i--) {
            for (int j = data[i].length - 1; j >= 0; j--) {
                System.out.printf("%d ", data[i][j]);
            }
            System.out.println();
        } 

        lsc.close();
    }
}
