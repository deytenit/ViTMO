package game;

import java.util.InputMismatchException;
import java.util.Scanner;

public class HumanPlayer implements Player {
    private final Scanner in;

    public HumanPlayer(Scanner in) {
        this.in = in;
    }

    @Override
    public Move makeMove(MNBoard mnBoard) {
        while (true) {
            System.out.println();
            System.out.println("Current position");
            System.out.println(mnBoard);
            System.out.println("Enter you move for " + mnBoard.getTurn());
            try {
                final Move move = new Move(in.nextInt() - 1, in.nextInt() - 1, mnBoard.getTurn());
                if (mnBoard.isValid(move)) {
                    return move;
                }
            }
            catch (InputMismatchException ignored) {}
            finally {
                in.nextLine();
                System.out.println("Not a valid move was performed! Try again...");
            }

        }
    }
}
