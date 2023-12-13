package ru.itmo.wp.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.*;
import ru.itmo.wp.form.DisabledForm;
import ru.itmo.wp.form.validator.DisabledFormDisableValidator;
import ru.itmo.wp.service.UserService;

import javax.servlet.http.HttpSession;
import javax.validation.Valid;

@Controller
public class UsersPage extends Page {
    private final UserService userService;
    private final DisabledFormDisableValidator disabledFormDisableValidator;

    public UsersPage(UserService userService, DisabledFormDisableValidator disabledFormDisableValidator) {
        this.userService = userService;
        this.disabledFormDisableValidator = disabledFormDisableValidator;
    }

    @InitBinder("disabledForm")
    public void initBinder(WebDataBinder binder) {
        binder.addValidators(disabledFormDisableValidator);
    }

    @GetMapping("/users/all")
    public String usersGet(Model model) {
        model.addAttribute("users", userService.findAll());
        return "UsersPage";
    }

    @PostMapping("/users/all")
    public String register(@Valid @ModelAttribute("disabledForm") DisabledForm disableForm,
                           BindingResult bindingResult,
                           HttpSession httpSession) {
        userService.updateDisabled(disableForm.getId(), disableForm.isDisabled());

        if (bindingResult.hasErrors()) {
            setMessage(httpSession, "Cannot change visibility for this user.");
        }

        setMessage(httpSession, "User visibility has been changed!");

        return "redirect:/users/all";
    }
}
