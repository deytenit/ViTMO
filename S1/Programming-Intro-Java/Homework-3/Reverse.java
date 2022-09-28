import java.util.Scanner;

public class Reverse {
    public static void main(String[] args) {
        Scanner lsc = new Scanner(System.in);
        
        int[] data = new int[1_000_000];
        int[] term = new int[1_000_000];

        int bufferSize = 0;
        int termSize = 1;

        while (lsc.hasNextLine()) {
            Scanner nsc = new Scanner(lsc.nextLine());

            while (nsc.hasNextInt()) {
                data[bufferSize] = nsc.nextInt();
                bufferSize++;
            }

            nsc.close();

            term[termSize] = bufferSize; 
            termSize++;
        }

        for (int i = termSize - 2; i >= 0; i--) {
            for (int j = term[i + 1] - 1; j >= term[i]; j--) {
                System.out.printf("%d ", data[j]);
            }
            System.out.println();
        } 

        lsc.close();
    }
}