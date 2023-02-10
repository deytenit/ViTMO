package game;

public interface Board {
    MNBoard getMNBoard();

    GameResult makeMove(Move move);

    void clean();
}
