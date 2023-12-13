package ru.itmo.wp.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.InitBinder;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import ru.itmo.wp.form.NoticeForm;
import ru.itmo.wp.form.validator.NoticeFormAddValidator;
import ru.itmo.wp.service.NoticeService;

import javax.servlet.http.HttpSession;
import javax.validation.Valid;

@Controller
public class NoticePage extends Page {
    private final NoticeService noticeService;
    private final NoticeFormAddValidator noticeFormAddValidator;

    public NoticePage(NoticeService noticeService, NoticeFormAddValidator noticeFormAddValidator) {
        this.noticeService = noticeService;
        this.noticeFormAddValidator = noticeFormAddValidator;
    }

    @InitBinder("noticeForm")
    public void initBinder(WebDataBinder binder) {
        binder.addValidators(noticeFormAddValidator);
    }

    @GetMapping("/notice")
    public String notice(Model model) {
        model.addAttribute("noticeForm", new NoticeForm());
        return "NoticePage";
    }

    @PostMapping("/notice")
    public String add(@Valid @ModelAttribute("noticeForm") NoticeForm noticeForm,
                      BindingResult bindingResult,
                      HttpSession httpSession) {
        if (bindingResult.hasErrors()) {
            return "NoticePage";
        }

        noticeService.add(noticeForm);

        setMessage(httpSession, "Notice added!");

        return "redirect:";
    }
}
