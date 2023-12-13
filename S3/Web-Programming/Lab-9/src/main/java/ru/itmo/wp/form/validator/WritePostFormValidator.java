package ru.itmo.wp.form.validator;

import org.springframework.stereotype.Component;
import org.springframework.validation.Errors;
import org.springframework.validation.Validator;
import ru.itmo.wp.form.UserCredentials;
import ru.itmo.wp.form.WritePostForm;
import ru.itmo.wp.service.UserService;

import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Component
public class WritePostFormValidator implements Validator {
    static final Pattern TAGS_REGEX = Pattern.compile("^[a-zA-Z\\s]*$");

    public WritePostFormValidator() {}

    public boolean supports(Class<?> clazz) {
        return WritePostForm.class.equals(clazz);
    }

    public void validate(Object target, Errors errors) {
        if (!errors.hasErrors()) {
            WritePostForm form = (WritePostForm) target;
            if (!TAGS_REGEX.matcher(form.getTags()).matches()) {
                errors.rejectValue("tags", "tags.contains-illegal-characters", "tags can only contain latin letters");
            }
        }
    }
}
