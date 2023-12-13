package ru.itmo.wp.model.domain;

import java.util.Map;

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

    public enum Type {
        ENTER, LOGOUT
    }
}
