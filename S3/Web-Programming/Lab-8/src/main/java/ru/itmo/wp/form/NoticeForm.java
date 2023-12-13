package ru.itmo.wp.form;

import javax.persistence.Lob;
import javax.validation.constraints.*;

@SuppressWarnings("unused")
public class NoticeForm {
    @Lob
    @NotNull
    @NotEmpty
    @NotBlank
    @Size(max = 255)
    private String content;

    public String getContent() {
        return content;
    }

    public void setContent(final String content) {
        this.content = content;
    }
}
