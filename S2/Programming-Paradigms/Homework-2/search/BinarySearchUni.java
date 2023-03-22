package search;

import java.util.Arrays;
import java.util.stream.IntStream;

public class BinarySearchUni {
    // Contract:
    // |    Pre:
    // |    |   args.length > 0
    // |    |   && for any 0 <= i < args.length -> Integer.MIN_VALUE <= args[i] <= Integer.MAX_VALUE
    // |    |   && Exist k For any 0 <= i < j < k -> arr[i] <= arr[j] && For any k <= i < j < arr.length -> arr[i] >= arr[j]
    // |    |   && Assuming: args[-1] = +inf && arr[args.length] = -inf
    // |    Post:
    // |    |   R == r : arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]
    public static void main(final String[] args) {
        final int[] arr = toInts(args);

        final int[] data = Arrays.copyOfRange(arr, 0, arr.length);

        System.out.println(
                IntStream.of(data).sum() % 2 == 0 ? recursive(data, -1, data.length - 1) : iterative(data)
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
    // |    |   -1 <= l < r < arr.length
    // |    |   && arr[l - 1] <= arr[l] && arr[r] > arr[r + 1]
    // |    Post:
    // |    |   R == r : arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]
    // I:
    // |    Exist k For any 0 <= i < j < k -> arr[i] <= arr[j] && For any k <= i < j < arr.length -> arr[i] >= arr[j]
    // |    Assuming: arr[-1] == arr[arr.length] == -inf
    public static int recursive(final int[] arr, final int l, final int r) {
        // Pre:
        // |    -1 <= l < r < arr.length
        // |    && arr[l] <= arr[l + 1] && arr[r] > arr[r + 1]
        if (r - l <= 1) {
            // r - l <= 1 -> r - 1 <= l
            // r - 1 <= l  && l < r -> l == r - 1

            // arr[l] <= arr[l + 1] && arr[r] > arr[r + 1] && l == r - 1 -> arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]

            return r;
            // R == r : arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]
        }

        final int mid = (l + r) / 2;
        // l <= mid == l + (r - l) / 2 < r

        // Pre:
        // |    l <= mid < r
        // |    && arr[l] <= arr[l + 1] && arr[r] > arr[r + 1]
        if ( arr[mid] > arr[mid + 1]) {
            // arr[mid] > arr[mid + 1]
            // l <= mid < r && arr[l] <= arr[l + 1] && arr[r] > arr[r + 1] && arr[mid] > arr[mid + 1]
            //      -> arr[l] <= arr[l + 1] && arr[mid] > arr[mid + 1]

            // arr[l] <= arr[l + 1] && arr[r == mid] > arr[(r == mid) + 1]
            // -1 <= l < r == mid < arr.length

            return recursive(arr, l, mid);
            // R == r : arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]
        }
        else {
            // arr[mid] <= arr[mid + 1]
            // l <= mid < r && arr[l] <= arr[l + 1] && arr[r] > arr[r + 1] && arr[mid] <= arr[mid + 1]
            //      -> arr[mid] <= arr[mid + 1] && arr[r] > arr[r + 1]

            // arr[l == mid] <= arr[(l == mid) + 1] && arr[r] > arr[r + 1]
            // -1 <= l < r == mid < arr.length

            return recursive(arr, mid, r);
            // R == r : arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]
        }
    }

    // Contract:
    // |    Pre:
    // |    |   Exists k For any 0 <= i < j < k -> arr[i] <= arr[j]
    // |    |   && For any k <= i < j < arr.length -> arr[i] >= arr[j]
    // |    Post:
    // |    |   R == r : arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]
    // I:
    // |    Assuming: arr[-1] == arr[arr.length] == -inf
    public static int iterative(final int[] arr) {
        int l = -1;
        int r = arr.length - 1;
        // l == -1 && r == arr.length - 1 && arr[l] <= arr[k] <= arr[k + 1] <= arr[r]

        // I:
        //      -1 <= l < r < arr.length && arr[l] <= arr[l + 1] && arr[r] > arr[r + 1]
        while (r - l > 1) {
            // -1 <= l < r < arr.length
            final int mid = l + (r - l) / 2;
            // l <= mid == l + (r - l) / 2 < r

            // Pre:
            // |    arr[l] <= arr[l + 1] && arr[r] > arr[r + 1]
            // |    && l <= mid < r
            if (arr[mid] <= arr[mid + 1]) {
                // arr[mid] <= arr[mid + 1]
                l = mid;
                // l' == mid
                // r' == r
                // arr[l'] <= arr[l' + 1] && arr[r'] > arr[r' + 1]
            }
            else {
                // arr[mid] > arr[mid + 1]
                r = mid;
                // l' = l
                // r' = mid
                // arr[l'] <= arr[l' + 1] && arr[r'] > arr[r' + 1]
            }
            // Post:
            // |    arr[l'] <= arr[l' + 1] && arr[r'] > arr[r' + 1]
        }
        // 0 <= l' < r' < arr.length
        // |    l' < r' && r' - l' <= 1 -> r' == l' + 1
        // |    -1 <= l' < r -> 0 <= r'
        // |    0 <= r' && r' == l' + 1 && r' < arr.length -> 0 <= r' == l' + 1 < arr.length
        //
        // arr[l'] <= arr[l' + 1] && arr[r'] > arr[r' + 1]
        // |    r' == l' + 1 -> l' == r' - 1
        // |    arr[r' - 1] <= arr[r'] && arr[r'] > arr[r' + 1]

        // 0 <= r' < arr.length && arr[r' - 1] <= arr[r'] && arr[r'] > arr[r' + 1]

        return r;
        // R == r : arr[r - 1] <= arr[r] && arr[r] > arr[r + 1]
    }
}
