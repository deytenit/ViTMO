package ru.itmo.wp.form.validator;

import org.springframework.stereotype.Component;
import org.springframework.validation.Errors;
import org.springframework.validation.Validator;
import ru.itmo.wp.form.DisabledForm;
import ru.itmo.wp.service.UserService;

@Component
public class DisabledFormDisableValidator implements Validator {
    private final UserService userService;

    public DisabledFormDisableValidator(UserService userService) {
        this.userService = userService;
    }

    public boolean supports(Class<?> clazz) {
        return DisabledForm.class.equals(clazz);
    }

    public void validate(Object target, Errors errors) {
        if (!errors.hasErrors()) {
            DisabledForm disableForm = (DisabledForm) target;
            if (userService.findById(disableForm.getId()) == null) {
                errors.rejectValue("id", "id.not-exists", "User with such id doesn't exists");
            }
        }
    }
}
