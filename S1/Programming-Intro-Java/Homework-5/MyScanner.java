import java.io.*;
import java.util.*;
import java.util.function.Predicate;

public class MyScanner {
    private final Predicate<Character> newLineDelimiter = c -> c == '\n' || c == '\r';

    private Reader source;
    private int position;
    private char[] buffer;
    private int bufferSize;

    private boolean tokenized;
    private boolean closed;
    private boolean exhausted;

    private IOException lastIOE;

    private boolean backedTokenized;
    private int backedPosition;

    private Predicate<Character> delimiter;

    public MyScanner(Reader source) {
        this.source = source;
        position = 0;
        buffer = new char[1];
        bufferSize = 0;

        tokenized = false;
        closed = false;
        exhausted = false;

        delimiter = Character::isWhitespace;
    }

    public MyScanner(InputStream source) {
        this(new InputStreamReader(source));
    }

    public MyScanner(InputStream source, String charset) throws UnsupportedEncodingException {
        this(new InputStreamReader(source, charset));
    }

    public MyScanner(File source) throws FileNotFoundException {
        this(new FileInputStream(source));
    }

    public MyScanner(File source, String charset) throws FileNotFoundException, UnsupportedEncodingException {
        this(new FileInputStream(source), charset);
    }

    public MyScanner(String source) {
        this(new StringReader(source));
    }

    private void backupState() {
        backedPosition = position;
        backedTokenized = tokenized;
    }

    private void revertState() {
        position = backedPosition;
        tokenized = backedTokenized;
        collapseBuffer(position);
    }

    public IOException IOException() {
        return lastIOE;
    }

    private void verify() {
        if (closed) {
            throw new IllegalStateException("Scanner is closed.");
        }
    }

    public void close() {
        if (closed) {
            return;
        }

        try {
            source.close();
        }
        catch (IOException e) {
            lastIOE = e;
        }

        source = null;
        closed = true;
    }

    public void setPredicate(Predicate<Character> prdc) {
        delimiter = prdc;
    }

    private void proceedToToken(Predicate<Character> prdc) {
        while (position < bufferSize && prdc.test(buffer[position])) {
            position++;
        }
    }

    private String nextToken(Predicate<Character> prdc) {
        tokenized = false;

        if (position == bufferSize && exhausted) {
            tokenized = true;
            return null;
        }

        int nextPosition = position;

        while (nextPosition < bufferSize && !prdc.test(buffer[nextPosition])) {
            nextPosition++;
        }

        if (nextPosition < bufferSize || exhausted) {
            tokenized = true;
            String result = String.valueOf(buffer, position, nextPosition - position);
            position = nextPosition;
            return result;
        }

        return null;
    }

    private void readSource() {
        if (bufferSize == buffer.length) {
            extendBuffer();
        }

        int bufferLimit = 1024;
        int limit = Math.min(buffer.length - bufferSize, bufferLimit);

        int state;

        try {
            state = source.read(buffer, bufferSize, limit);
        }
        catch (IOException e) {
            lastIOE = e;
            state = -1;
        }

        if (state == -1) {
            exhausted = true;
            return;
        }

        bufferSize += state;
    }

    private void extendBuffer() {
        buffer = Arrays.copyOf(buffer, bufferSize * 2);
    }

    private void collapseBuffer(int position) {
        buffer = Arrays.copyOfRange(buffer, position, bufferSize);
        this.position = 0;
        bufferSize = buffer.length;
    }

    private boolean isInteger(String value, int radix) {
        if (value.isEmpty()) {
            return false;
        }

        for (int i = 0; i < value.length(); i++) {
            if (i == 0 && value.charAt(i) == '-') {
                if (value.length() == 1) {
                    return false;
                }
                else {
                    continue;
                }
            }

            if (Character.digit(value.charAt(i), radix) < 0) {
                return false;
            }
        }
        return true;
    }

    public boolean hasNext() throws IllegalStateException {
        return hasNext(delimiter);
    }

    public boolean hasNext(Predicate<Character> prdc) throws IllegalStateException {
        verify();

        backupState();

        while (!exhausted) {
            proceedToToken(prdc);
            if (nextToken(prdc) != null) {
                revertState();
                return true;
            }

            readSource();
        }

        proceedToToken(prdc);
        boolean state = nextToken(prdc) != null;
        revertState();

        return state;
    }

    public String next() throws NoSuchElementException, IllegalStateException {
        return next(delimiter);
    }

    public String next(Predicate<Character> prdc) throws NoSuchElementException, IllegalStateException {
        verify();

        while (true) {
            proceedToToken(prdc);
            String result = nextToken(prdc);

            if (result != null) {
                return result;
            }

            if (!tokenized) {
                readSource();
            }
            else {
                throw new NoSuchElementException("Buffer is exhausted.");
            }
        }
    }

    public boolean hasNextLine() throws IllegalStateException {
        verify();

        /*if (bufferSize == buffer.length) readSource();

        return bufferSize < buffer.length || !exhausted;*/

        return hasNext(newLineDelimiter.negate()) || hasNext(newLineDelimiter);
    }

    public String nextLine() throws IllegalStateException, NoSuchElementException {
        verify();

        while (true) {
            String result = nextToken(newLineDelimiter);

            if (result != null) {
                if (position < bufferSize) {
                    position += bufferSize - position > 1 && buffer[position] == '\r' && buffer[position + 1] == '\n' ? 2 : 1;
                }
                return result;
            }

            if (!tokenized) {
                readSource();
            }
            else {
                throw new NoSuchElementException("Buffer is exhausted.");
            }
        }
    }

    public boolean hasNextInt() throws IllegalStateException {
        return hasNextInt(10);
    }

    public boolean hasNextInt(int radix) throws IllegalStateException {
        verify();

        if (!hasNext()) {
            return false;
        }

        backupState();

        boolean state = isInteger(next(), radix);

        revertState();

        return state;

    }

    public int nextInt() throws IllegalStateException, InputMismatchException, NoSuchElementException {
        return nextInt(10);
    }

    public int nextInt(int radix) throws IllegalStateException, InputMismatchException, NoSuchElementException {
        verify();

        backupState();

        try {
            String token = next();

            return Integer.parseInt(token, radix);
        }
        catch (NumberFormatException e) {
            revertState();
            throw new InputMismatchException("Token is not integer.");
        }
    }
}