package ru.itmo.wp.form.validator;

import org.springframework.stereotype.Component;
import org.springframework.validation.Errors;
import org.springframework.validation.Validator;
import ru.itmo.wp.form.NoticeForm;
import ru.itmo.wp.service.NoticeService;

@Component
public class NoticeFormAddValidator implements Validator {
    private final NoticeService noticeService;

    public NoticeFormAddValidator(NoticeService noticeService) {
        this.noticeService = noticeService;
    }

    public boolean supports(Class<?> clazz) {
        return NoticeForm.class.equals(clazz);
    }

    public void validate(Object target, Errors errors) {
        if (!errors.hasErrors()) {
            NoticeForm addForm = (NoticeForm) target;
            if (noticeService.hasBadWords(addForm.getContent())) {
                errors.rejectValue("content", "content.has-bad-words", "Content can't contain bad words");
            }
        }
    }
}
