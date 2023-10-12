package ru.itmo.web.hw4.util;

public enum Color {
    RED("red"),
    GREEN("green"),
    BLUE("blue");

    private final String css;
    Color(String css) {
        this.css = css;
    }

    public String getCss() {
        return css;
    }
}
