package ru.itmo.wp.web.page;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.Arrays;
import java.util.Map;

@SuppressWarnings({"unused", "RedundantSuppression"})
public class TicTacToePage {
    private void action(HttpServletRequest request, Map<String, Object> view) {
        State game = State.get(request.getSession());
        game.set(request.getSession(), view);
    }

    private void newGame(HttpServletRequest request, Map<String, Object> view) {
        State game = new State();
        game.set(request.getSession(), view);
    }

    private void onMove(HttpServletRequest request, Map<String, Object> view) {
        State game = State.get(request.getSession());

        try {
            final int row = Integer.parseInt(request.getParameter("row"));
            final int col = Integer.parseInt(request.getParameter("col"));

            game.doMove(row, col);
        }
        catch (NumberFormatException | IndexOutOfBoundsException ignored) {}

        game.set(request.getSession(), view);
    }

    public enum Cell {
        X,
        O
    }

    public enum Phase {
        RUNNING,
        WON_X,
        WON_O,
        DRAW
    }

    public static class State {
        private final int size = 3;
        private final Cell[][] cells = new Cell[size][size];
        private Phase phase = Phase.RUNNING;
        private boolean crossesMove = true;
        private int move = 0;

        public int getSize() {
            return size;
        }

        public Cell[][] getCells() {
            return cells;
        }

        public Phase getPhase() {
            return phase;
        }

        public boolean isCrossesMove() {
            return crossesMove;
        }

        public void doMove(int row, int col) {
            if (cells[row][col] != null) {
                return;
            }

            cells[row][col] = crossesMove ? Cell.X : Cell.O;
            ++move;

            checkState(row, col);

            crossesMove = !crossesMove;
        }

        public void set(HttpSession session, Map<String, Object> view) {
            session.setAttribute("game", this);
            view.put("state", this);
        }

        private void checkState(int row, int col) {
            final int[] strikeouts = new int[]{
                    countCells(row, col, 1, 0) + countCells(row, col, -1, 0) + 1,
                    countCells(row, col, 0, 1) + countCells(row, col, 0, -1) + 1,
                    countCells(row, col, 1, 1) + countCells(row, col, -1, -1) + 1,
                    countCells(row, col, 1, -1) + countCells(row, col, -1, 1) + 1
            };

            if (Arrays.stream(strikeouts).anyMatch(x -> x == size)) {
                phase = crossesMove ? Phase.WON_X : Phase.WON_O;
            }
            else if (move == size * size) {
                phase = Phase.DRAW;
            }
        }

        private int countCells(int row, int col, int offsetX, int offsetY) {
            final Cell cell = cells[row][col];

            int cnt = 0;
            do {
                ++cnt;
                row += offsetX;
                col += offsetY;
            } while (inRange(row, col) && cells[row][col] != null && cell == cells[row][col]);

            return cnt - 1;
        }

        private boolean inRange(int row, int col) {
            return 0 <= row && row < size
                    && 0 <= col && col < size;
        }

        public static State get(HttpSession session) {
            State game = (State)session.getAttribute("game");

            if (game == null) {
                game = new State();
            }

            return game;
        }
    }
}
