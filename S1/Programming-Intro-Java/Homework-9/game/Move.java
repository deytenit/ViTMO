package game;

public record Move(int row, int col, Cell value) {

    @Override
    public String toString() {
        return String.format("Move(%s, %d, %d)", value, row + 1, col + 1);
    }
}
