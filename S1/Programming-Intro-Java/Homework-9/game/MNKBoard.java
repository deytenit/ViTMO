package game;

import java.util.Arrays;

public class MNKBoard extends MNBoard implements Board {
    private final int condition;
    Move[] baseMoves;

    int filled = 0;

    public MNKBoard(int length, int width, int condition, Move[] baseMoves) {
        super(length, width);

        for (Move move: baseMoves) {
            field[move.row()][move.col()] = move.value();
            if (move.value() != Cell.E) {
                filled++;
            }
        }

        this.baseMoves = baseMoves;
        this.condition = condition;
    }

    @Override
    public MNBoard getMNBoard() {
        return new MNBoard(field, turn);
    }

    @Override
    public GameResult makeMove(Move move) {
        if (!isValid(move)) {
            return GameResult.LOOSE;
        }

        field[move.row()][move.col()] = move.value();

        filled++;

        if (checkWin(move)) {
            return GameResult.WIN;
        }

        if (checkDraw()) {
            return GameResult.DRAW;
        }

        turn = turn == Cell.X ? Cell.O : Cell.X;

        return GameResult.UNKNOWN;
    }

    private boolean checkDraw() {
        return filled == field.length * field[0].length;
    }

    private int checkTo(Move pos, int incX, int incY) {
        int count = 0;

        int row = pos.row();
        int col = pos.col();

        Cell baseCell = field[row][col];

        while (isInRange(new Move(row, col, baseCell)) && field[row][col] == baseCell) {
            count++;

            row += incX;
            col += incY;
        }

        return count;
    }

    private boolean checkWin(Move move) {
        return checkTo(move, 1, 0) + checkTo(move, -1, 0) - 1 == condition
                || checkTo(move, 0, 1) + checkTo(move, 0, -1) - 1 == condition
                || checkTo(move, 1, 1) + checkTo(move, -1, -1) - 1 == condition
                || checkTo(move, 1, -1) + checkTo(move, -1, 1) - 1 == condition;
    }

    @Override
    public void clean() {
        filled = 0;

        for (Cell[] cells : field) {
            Arrays.fill(cells, Cell.E);
        }

        for (Move move: baseMoves) {
            field[move.row()][move.col()] = move.value();
            if (move.value() != Cell.E) {
                filled++;
            }
        }
    }
}
