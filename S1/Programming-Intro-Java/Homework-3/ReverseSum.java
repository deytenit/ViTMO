import java.util.Arrays;
import java.util.Scanner;

public class ReverseSum {
    public static void main(String[] args) {
        Scanner lsc = new Scanner(System.in);
        
        int termSize = 1;
        int[] term = new int[2];
        int dataSize = 0;
        int[] data = new int[1];

        int maxRowSize = 0;

        while (lsc.hasNextLine()) {
            Scanner nsc = new Scanner(lsc.nextLine());
            
            while (nsc.hasNextInt()) {
                if (data.length == dataSize) {
                    data = Arrays.copyOf(data, data.length * 2);
                }

                data[dataSize++] = nsc.nextInt();
                //dataSize++;
            }
            
            if (term.length == termSize) {
                term = Arrays.copyOf(term, term.length * 2);
            }
            
            term[termSize] = dataSize; 
            termSize++;

            maxRowSize = Math.max(maxRowSize, term[termSize - 1] - term[termSize - 2]);
        }

        int[] rowSum = new int[termSize]; 
        int[] colSum = new int[maxRowSize];

        for (int i = 0; i < termSize - 1; i++) {
            for (int j = term[i]; j < term[i + 1]; j++) {
                rowSum[i] += data[j];
                colSum[j - term[i]] += data[j];
            }
        } 

        for (int i = 0; i < termSize - 1; i++) {
            for (int j = term[i]; j < term[i + 1]; j++) {
                System.out.printf("%d ", rowSum[i] + colSum[j - term[i]] - data[j]);
            }
            System.out.println();
        } 

        lsc.close();
    }
}
