package ru.itmo.wp.model.domain;

import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Article extends Entity implements SpecializedEntity {
    private long userId;
    private String title;
    private String text;
    private boolean hidden;

    public long getUserId() {
        return userId;
    }

    public void setUserId(final long userId) {
        this.userId = userId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(final String title) {
        this.title = title;
    }

    public String getText() {
        return text;
    }

    public void setText(final String text) {
        this.text = text;
    }

    public boolean isHidden() {
        return hidden;
    }

    public void setHidden(final boolean hidden) {
        this.hidden = hidden;
    }

    @Override
    public Map<String, Object> getSpecializedEntries() {
        return Map.of(
            "userId", userId,
            "title", title,
            "text", text,
            "hidden", hidden
        );
    }

    @Override
    public Map<String, Object> getEntries() {
        return Stream.concat(super.getEntries().entrySet().stream(), getSpecializedEntries().entrySet().stream())
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    }
}
