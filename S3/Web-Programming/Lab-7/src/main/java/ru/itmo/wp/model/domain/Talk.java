package ru.itmo.wp.model.domain;

import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Talk extends Entity implements SpecializedEntity {
    private long sourceUserId;
    private long targetUserId;
    private String text;

    public long getSourceUserId() {
        return sourceUserId;
    }

    public void setSourceUserId(long sourceUserId) {
        this.sourceUserId = sourceUserId;
    }

    public long getTargetUserId() {
        return targetUserId;
    }

    public void setTargetUserId(long targetUserId) {
        this.targetUserId = targetUserId;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    @Override
    public Map<String, Object> getSpecializedEntries() {
        return Map.of("sourceUserId", sourceUserId, "targetUserId", targetUserId, "text", text);
    }

    @Override
    public Map<String, Object> getEntries() {
        return Stream.concat(super.getEntries().entrySet().stream(), getSpecializedEntries().entrySet().stream())
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    }
}
