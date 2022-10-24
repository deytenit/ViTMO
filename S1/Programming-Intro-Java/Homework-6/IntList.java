import java.util.Arrays;

public class IntList {
    private int[] data = new int[1];
    private int dataSize = 0;

    public int size() {
        return  dataSize;
    }

    public void add(int x) {
        if (dataSize == data.length) {
            data = Arrays.copyOf(data, dataSize * 2);
        }

        data[dataSize++] = x;
    }

    public void pop() {
        if (dataSize == 0) {
            return;
        }

        dataSize--;
    }

    public int get(int pos) {
        if (pos >= dataSize) {
            throw new IndexOutOfBoundsException("pos argument should be less than container size.");
        }

        return data[pos];
    }

    public void set(int pos, int val) {
        if (pos >= dataSize) {
            throw new IndexOutOfBoundsException("pos argument should be less than container size.");
        }

        data[pos] = val;
    }
}
