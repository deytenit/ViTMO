package game;

import java.util.Arrays;

public class TournamentGame {
    private final Board board;
    private final Player[] players;


    public TournamentGame(Board board, Player[] players) {
        this.board = board;
        this.players = players;
    }

    public int play(boolean log) {
        PairOfIntegers[] result = new PairOfIntegers[players.length];

        for (int i = 0; i < result.length; i++) {
            result[i] = new PairOfIntegers(0, i);
        }

        for (int i = 0; i < players.length; i++) {
            for (int j = 0; j < players.length; j++) {
                if (i == j) {
                    continue;
                }

                board.clean();

                System.out.println("Player " + (i + 1) + " and " + (j + 1) + " ready to play!");

                while (true) {
                    final int result1 = makeMove(players[i], i + 1, log);
                    if (result1 != -1)  {
                        result[i].first += result1 == 1 ? 3 : result1 == 0 ? 1 : 0;
                        result[j].first += result1 == 2 ? 3 : result1 == 0 ? 1 : 0;
                        break;
                    }
                    final int result2 = makeMove(players[j], j + 1, log);
                    if (result2 != -1)  {
                        result[j].first += result2 == 1 ? 3 : result2 == 0 ? 1 : 0;
                        result[i].first += result2 == 2 ? 3 : result2 == 0 ? 1 : 0;
                        break;
                    }
                }
            }
        }

        Arrays.sort(result);

        for (int i = 0; i < result.length; i++) {
            System.out.println("#" + (i + 1) + " | Player: " + (result[i].second + 1) + " | " + result[i].first + " Pts.");
        }

        return result[0].second + 1;
    }

    private int makeMove(Player player, int no, boolean log) {
        final Move move = player.makeMove(board.getMNBoard());
        final GameResult result = board.makeMove(move);
        if (log) {
            System.out.println();
            System.out.println("Player: " + no);
            System.out.println(move);
            System.out.println(board);
            System.out.println("Result: " + result);
        }
        return switch (result) {
            case WIN -> 1;
            case LOOSE -> 2;
            case DRAW -> 0;
            case UNKNOWN -> -1;
        };
    }

    private static class PairOfIntegers implements Comparable<PairOfIntegers> {
        public int first;
        public int second;

        public PairOfIntegers(int first, int second) {
            this.first = first;
            this.second = second;
        }

        @Override
        public int compareTo(PairOfIntegers pair) {
            return first == pair.first ? second - pair.second : pair.first - first;
        }
    }
}
