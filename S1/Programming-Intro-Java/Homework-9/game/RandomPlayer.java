package game;

import java.util.Random;

public class RandomPlayer implements Player {
    private final Random random = new Random();

    final int lengthBorder;
    final int widthBorder;

    public RandomPlayer(final int lengthBorder, final int widthBorder) {
        this.lengthBorder = lengthBorder;
        this.widthBorder = widthBorder;
    }

    @Override
    public Move makeMove(MNBoard MNBoard) {
        while (true) {
            final Move move = new Move(
                    random.nextInt(lengthBorder),
                    random.nextInt(widthBorder),
                    MNBoard.getTurn()
            );
            if (MNBoard.isValid(move)) {
                return move;
            }
        }
    }
}
