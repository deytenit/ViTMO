package ru.itmo.wp.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.InitBinder;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import ru.itmo.wp.domain.Post;
import ru.itmo.wp.domain.Role;
import ru.itmo.wp.domain.Tag;
import ru.itmo.wp.form.WritePostForm;
import ru.itmo.wp.form.validator.WritePostFormValidator;
import ru.itmo.wp.security.AnyRole;
import ru.itmo.wp.service.PostService;
import ru.itmo.wp.service.TagService;
import ru.itmo.wp.service.UserService;

import javax.servlet.http.HttpSession;
import javax.validation.Valid;

@Controller
public class WritePostPage extends Page {
    private final PostService postService;
    private final TagService tagService;
    private final WritePostFormValidator writePostFormValidator;

    public WritePostPage(PostService postService, TagService tagService, WritePostFormValidator writePostFormValidator) {
        this.postService = postService;
        this.tagService = tagService;
        this.writePostFormValidator = writePostFormValidator;
    }

    @InitBinder("postForm")
    public void initBinder(WebDataBinder binder) {
        binder.addValidators(writePostFormValidator);
    }

    @AnyRole({Role.Name.WRITER, Role.Name.ADMIN})
    @GetMapping("/writePost")
    public String writePostGet(Model model) {
        model.addAttribute("postForm", new WritePostForm());
        return "WritePostPage";
    }

    @AnyRole({Role.Name.WRITER, Role.Name.ADMIN})
    @PostMapping("/writePost")
    public String writePostPost(@Valid @ModelAttribute("postForm") WritePostForm form,
                                BindingResult bindingResult,
                                HttpSession httpSession) {
        if (bindingResult.hasErrors()) {
            return "WritePostPage";
        }

        Post post = new Post();
        post.setTitle(form.getTitle());
        post.setText(form.getText());
        post.setUser(getUser(httpSession));
        post.setTags(tagService.generate(form.getTags()));

        postService.writePost(post);
        putMessage(httpSession, "You published new post");

        return "redirect:/myPosts";
    }
}
