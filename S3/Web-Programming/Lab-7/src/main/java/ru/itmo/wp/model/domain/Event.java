package ru.itmo.wp.model.domain;

import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Event extends Entity implements SpecializedEntity {
    private long userId;
    private Type type;

    public long getUserId() {
        return userId;
    }

    public void setUserId(long userId) {
        this.userId = userId;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }

    @Override
    public Map<String, Object> getSpecializedEntries() {
        return Map.of("userId", userId, "type", type);
    }

    @Override
    public Map<String, Object> getEntries() {
        return Stream.concat(super.getEntries().entrySet().stream(), getSpecializedEntries().entrySet().stream())
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    }

    public enum Type {
        ENTER, LOGOUT
    }
}
