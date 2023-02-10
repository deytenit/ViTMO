package game;

import java.util.Scanner;

public class Main {
    private static int ModeSelector(Scanner in) {
        System.out.println("Select mode:");
        System.out.println("1. Duel");
        System.out.println("2. Tournament");

        return switch (in.nextInt()) {
            case 1 -> 0;
            case 2 -> 1;
            default -> -1;
        };
    }

    private static Player PlayerSelector(Scanner in, int num, int fieldLength, int fieldWidth) {
        System.out.println("Assign player " + num + ":");
        System.out.println("1. Robota");
        System.out.println("2. The mortal one");

        return switch (in.nextInt()) {
            case 1 -> new RandomPlayer(fieldLength, fieldWidth);
            case 2 -> new HumanPlayer(in);
            default -> throw new IllegalStateException("Unexpected value for playerSelector provided.");
        };
    }

    public static void main(String[] args) {

        Scanner in = new Scanner(System.in);

        System.out.println("Assign field props:");
        System.out.print("length: ");
        final int fieldLength = in.nextInt();
        System.out.print("width: ");
        final int fieldWidth = in.nextInt();
        System.out.print("win condition: ");
        final int fieldCondition = in.nextInt();
        System.out.println("Obstacles amount: ");
        final int obstaclesAmount = in.nextInt();
        Move[] obstacles = new Move[obstaclesAmount];

        if (obstaclesAmount > 0) {
            System.out.println("Obstacles positions: ");
            for (int i = 0; i < obstaclesAmount; i++) {
                obstacles[i] = new Move(in.nextInt() - 1, in.nextInt() - 1, Cell.B);
            }
        }

        System.out.println("Welcome to the Mortal Neko Kombat or MNKGame:");

        int mode;

        while ((mode = ModeSelector(in)) == -1) {
            System.out.println("Not a valid mode was selected. Please try again...");
        }

        int playersAmount = 2;

        if (mode == 1) {
            while (true) {
                System.out.print("Enter amount of players:");
                playersAmount = in.nextInt();

                if (playersAmount > 1) {
                    break;
                }

                System.out.println("Not a valid mode was selected. Please try again...");
            }
        }

        Player[] players = new Player[playersAmount];

        for (int i = 0; i < playersAmount; i++) {
            players[i] = PlayerSelector(in, i + 1, fieldLength, fieldWidth);
        }
        

        final int result = switch (mode) {
            case 0 -> new TwoPlayerGame(
                    new MNKBoard(fieldLength, fieldWidth, fieldCondition, obstacles),
                    players[0],
                    players[1]
            ).play(true);
            case 1 -> new TournamentGame(
                    new MNKBoard(fieldLength, fieldWidth, fieldCondition, obstacles),
                    players
            ).play(true);
            default -> throw new IllegalStateException("Unexpected value: " + mode);
        };

        if (result == 0) {
            System.out.println("Draw");
        }
        else if (result > 0) {
            System.out.println("Player " + result + " won!");
        }
    }
}
