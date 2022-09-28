public class Sum {
    public static int splitSum(String str) {
        int sum = 0;

        int l = 0;
        int r = 0;
        while (r < str.length()) {
            if (Character.isWhitespace(str.charAt(l))) {
                l = r;
            }
            
            if (Character.isWhitespace(str.charAt(r)) && !Character.isWhitespace(str.charAt(l))) {
                sum += Integer.parseInt(str, l, r, 10);
                l = r;
            }

            r++;
        }

        if (!Character.isWhitespace(str.charAt(l))) {
            sum += Integer.parseInt(str, l, r, 10);
        }

        return sum;
    }

    public static void main(String[] args) {      
        int sum = 0;
        for (int i = 0; i < args.length; i++) {
            sum += splitSum(args[i]);
        }  

        System.out.println(sum);
    }
}