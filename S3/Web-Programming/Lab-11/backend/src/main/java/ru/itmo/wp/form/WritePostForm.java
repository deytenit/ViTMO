package ru.itmo.wp.form;

import org.hibernate.annotations.CreationTimestamp;
import ru.itmo.wp.domain.User;

import javax.persistence.*;
import javax.validation.constraints.*;
import java.util.Date;

public class WritePostForm {
    @NotBlank
    @Size(min = 1, max = 100)
    private String title;

    @NotBlank
    @Size(min = 1, max = 10000)
    @Lob
    private String text;

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
}
