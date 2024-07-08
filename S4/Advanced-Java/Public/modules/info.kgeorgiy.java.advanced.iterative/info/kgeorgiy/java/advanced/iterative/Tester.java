package info.kgeorgiy.java.advanced.iterative;

import info.kgeorgiy.java.advanced.base.BaseTester;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public final class Tester {
    private Tester() {
    }

    public static void main(final String... args) {
        new BaseTester()
                .add("scalar", ScalarIPTest.class)
                .add("list", ListIPTest.class)
                .add("new-scalar", NewScalarIPTest.class)
                .add("new-list", NewListIPTest.class)
                .add("advanced", AdvancedIPTest.class)
                .run(args);
    }
}
