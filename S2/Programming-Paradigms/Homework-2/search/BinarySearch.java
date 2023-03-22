package search;

import java.util.Arrays;
import java.util.stream.IntStream;

public class BinarySearch {
    // Contract:
    // |    Pre:
    // |    |   args.length > 0
    // |    |   && for any 0 <= i < args.length -> Integer.MIN_VALUE <= args[i] <= Integer.MAX_VALUE
    // |    |   && For any 0 <= i < j < args.length -> args[i] >= args[j]
    // |    |   && Assuming: args[-1] = +inf && arr[args.length] = -inf
    // |    Post:
    // |    |   R == r : arr[r - 1] > query >= [r]
    public static void main(final String[] args) {
        final int[] arr = toInts(args);

        final int query = arr[0];
        final int[] data = Arrays.copyOfRange(arr, 1, arr.length);

        System.out.println(
                IntStream.of(data).sum() % 2 == 0 ? recursive(data, -1, data.length, query) : iterative(data, query)
        );
    }

    // Contract:
    // |    Pre:
    // |    |   for any 0 <= i < args.length -> Integer.MIN_VALUE <= args[i] <= Integer.MAX_VALUE
    // |    Post:
    // |    |   for any 0 <= i < args.length -> Integer.MIN_VALUE <= args[i] <= Integer.MAX_VALUE
    public static int[] toInts(final String[] args) {
        int[] res = new int[args.length];

        for (int i = 0; i < args.length; ++i) {
            res[i] = Integer.parseInt(args[i]);
        }

        return res;
    }

    // Contract:
    // |    Pre:
    // |    |   arr[l] > query >= arr[r]
    // |    |   && -1 <= l < r <= arr.length
    // |    Post:
    // |    |   R == r : arr[r - 1] > query >= [r]
    // I:
    // |    For any 0 <= i < j < arr.length -> arr[i] >= arr[j]
    // |    && Assuming: arr[-1] = +inf && arr[arr.length] = -inf
    public static int recursive(final int[] arr, final int l, final int r, final int query) {
        // Pre:
        // |    arr[l] > query >= arr[r]
        // |    && -1 <= l < r <= arr.length
        if (r - l <= 1) {
            // r - l <= 1 -> r - 1 <= l
            // r - 1 <= l  && l < r -> l == r - 1
            return r;
            // R == r : arr[r - 1] > query >= [r]
        }

        final int mid = (l + r) / 2;
        // l <= mid == l + (r - l) / 2 < r

        // Pre:
        // |    arr[l] > query >= arr[r]
        // |    && l <= mid < r
        if (query >= arr[mid]) {
            // query >= arr[mid]
            // l <= mid < r && arr[l] > query >= arr[r] && query >= arr[mid] -> arr[l] > query >= arr[mid]

            // arr[l] > query >= arr[r == mid]
            // -1 <= l < r == mid <= arr.length
            return recursive(arr, l, mid, query);
            // R == r : arr[r - 1] > query >= [r]
        }
        else {
            // arr[mid] > query
            // l <= mid < r && arr[l] > query >= arr[r] && arr[mid] > query -> arr[mid] > query >= arr[r]

            // arr[l == mid] > query >= arr[r]
            // -1 <= l == mid < r <= arr.length
            return recursive(arr, mid, r, query);
            // R == r : arr[r - 1] > query >= [r]
        }
    }

    // Contract:
    // |    Pre:
    // |    |   arr[-1] >= query >= arr[arr.length]
    // |    Post:
    // |    |   R == r : arr[r - 1] > query >= [r]
    // I:
    // |    For any 0 <= i < j < arr.length -> arr[i] >= arr[j]
    // |    && Assuming: arr[-1] = +inf && arr[arr.length] = -inf
    public static int iterative(final int[] arr, final int query) {
        int l = -1;
        int r = arr.length;
        // l == -1 && r == arr.length && arr[l] > query >= arr[r]

        // I:
        //      -1 <= l < r <= arr.length && arr[l] > query >= arr[r]
        while (r - l > 1) {

            // -1 <= l < r <= arr.length
            final int mid = l + (r - l) / 2;
            // l <= mid == l + (r - l) / 2 < r

            // Pre:
            // |    arr[l] > query >= arr[r]
            // |    && l <= mid < r
            if (arr[mid] > query) {
                // arr[mid] > query
                l = mid;
                // l' == mid
                // r' == r
                // l <= l' < r && arr[l] > query >= arr[r] && arr[l'] > query -> arr[l'] > query >= arr[r']
            }
            else {
                // query >= arr[mid]
                r = mid;
                // l' = l
                // r' = mid
                // l <= r' < r && arr[l] > query >= arr[r] && query >= arr[r'] -> arr[l'] > query >= arr[r']
            }
            // Post:
            // |    arr[l'] > query >= arr[r']
        }
        // -1 <= l' < r' <= arr.length && arr[l'] > query >= arr[r'] && r' - l' <= 1
        //      l' < r' && r' - l' <= 1 -> r' == l' + 1
        //      -1 <= l' < r -> 0 <= r'
        //      0 <= r' && r' == l' + 1 && r' <= arr.length -> 0 <= r' == l' + 1 <= arr.length
        //
        // arr[r' - 1] > query >= [r'] && 0 <= r' <= arr.length

        return r;
        // R == r : arr[r - 1] > query >= [r]
    }
}
