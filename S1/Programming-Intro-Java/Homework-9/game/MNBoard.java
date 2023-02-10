package game;

import java.util.Arrays;
import java.util.Map;

public class MNBoard {
    private static final Map<Cell, String> CELL_TO_STRING = Map.of(
            Cell.E, ".",
            Cell.X, "X",
            Cell.O, "0",
            Cell.B, "-"
    );

    protected final Cell[][] field;

    protected Cell turn;

    public MNBoard(Cell[][] field, Cell turn) {
        this.field = new Cell[field.length][];

        for (int row = 0; row < field.length; row++) {
            this.field[row] = Arrays.copyOf(field[row], field[row].length);
        }
        this.turn = turn;
    }

    public MNBoard(int length, int width) {
        field = new Cell[length][width];

        for (Cell[] row : field) {
            Arrays.fill(row, Cell.E);
        }

        turn = Cell.X;
    }

    public Cell getTurn() {
        return turn;
    }

    public boolean isInRange(final Move move) {
        return 0 <= move.row() && move.row() < field.length
                && 0 <= move.col() && move.col() < field[0].length;
    }

    public boolean isValid(Move move) {
        return isInRange(move)
                && field[move.row()][move.col()] == Cell.E
                && turn == move.value();
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder();

        int maxLengthSize = String.valueOf(field.length).length();

        for (int widthSize = String.valueOf(field[0].length).length(); widthSize > 0; widthSize--) {
            sb.append(" ".repeat(maxLengthSize));

            int index = (int)Math.pow(10, widthSize - 1);

            sb.append(' ');
            sb.append("0".repeat(index - 1));

            for (int j = 1; j <= field[0].length / index; j++) {
                sb.append(String.valueOf(j % 10).repeat(Math.min(index, field[0].length - index * j + 1)));
            }
            sb.append(System.lineSeparator());
        }


        for (int r = 0; r < field.length; r++) {
            sb.append(" ".repeat(maxLengthSize - String.valueOf(r + 1).length()));
            sb.append(r + 1);
            sb.append(' ');
            for (Cell cell : field[r]) {
                sb.append(CELL_TO_STRING.get(cell));
            }
            sb.append(System.lineSeparator());
        }
        sb.setLength(sb.length() - System.lineSeparator().length());
        return sb.toString();
    }
}
