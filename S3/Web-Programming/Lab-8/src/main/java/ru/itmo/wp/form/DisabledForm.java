package ru.itmo.wp.form;

import javax.validation.constraints.NotNull;

@SuppressWarnings("unused")
public class DisabledForm {
    @NotNull
    private long id;

    @NotNull
    private boolean disabled;

    public long getId() {
        return id;
    }

    public void setId(final long id) {
        this.id = id;
    }

    public boolean isDisabled() {
        return disabled;
    }

    public void setDisabled(final boolean disabled) {
        this.disabled = disabled;
    }
}
