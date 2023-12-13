package ru.itmo.wp.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;
import ru.itmo.wp.domain.Comment;
import ru.itmo.wp.domain.Post;
import ru.itmo.wp.security.Guest;
import ru.itmo.wp.service.CommentService;
import ru.itmo.wp.service.PostService;

import javax.servlet.http.HttpSession;
import javax.validation.Valid;

@Controller
public class PostPage extends Page {
    private final PostService postService;
    private final CommentService commentService;

    public PostPage(PostService postService, CommentService commentService) {
        this.postService = postService;
        this.commentService = commentService;
    }

    @Guest
    @GetMapping(value={"/post/{id}", "/post"})
    public String postGet(Model model, @PathVariable(required = false) String id) {
        model.addAttribute("comment", new Comment());

        model.addAttribute("post", getPost(id));
        return "PostPage";
    }

    @PostMapping(value={"/post/{id}"})
    public String commentPost(@PathVariable String id, @Valid @ModelAttribute("comment") Comment comment,
                              BindingResult bindingResult,
                              HttpSession httpSession, Model model) {
        Post post = getPost(id);

        if (bindingResult.hasErrors()) {
            model.addAttribute("post", post);
            return "PostPage";
        }

        if (post == null) {
            return "PostPage";
        }

        comment.setUser(getUser(httpSession));
        comment.setPost(post);

        commentService.writeComment(comment);

        putMessage(httpSession, "Comment posted!");
        
        return "redirect:/post/" + id;
    }

    private Post getPost(String id) {
        try {
            return postService.findById(Long.parseLong(id));
        } catch (NumberFormatException ignored) {
            return null;
        }
    }
}
