package ru.itmo.wp.controller;

import org.springframework.data.jpa.repository.Query;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.web.bind.annotation.*;
import ru.itmo.wp.domain.Comment;
import ru.itmo.wp.domain.Post;
import ru.itmo.wp.domain.User;
import ru.itmo.wp.exception.NoSuchResourceException;
import ru.itmo.wp.exception.UnauthorizedException;
import ru.itmo.wp.exception.ValidationException;
import ru.itmo.wp.form.WritePostForm;
import ru.itmo.wp.service.CommentService;
import ru.itmo.wp.service.JwtService;
import ru.itmo.wp.service.PostService;

import javax.naming.AuthenticationException;
import javax.servlet.http.HttpSession;
import javax.validation.Valid;
import java.util.List;

@RestController
@RequestMapping("/api/1")
public class PostController {
    private final CommentService commentService;
    private final PostService postService;
    private final JwtService jwtService;

    public PostController(CommentService commentService, PostService postService, JwtService jwtService) {
        this.commentService = commentService;
        this.postService = postService;
        this.jwtService = jwtService;
    }

    @GetMapping("posts")
    public List<Post> findPosts() {
        return postService.findAll();
    }

    @PostMapping("posts")
    public Post writePost(@Valid @RequestBody WritePostForm postForm, @RequestParam String jwt, BindingResult bindingResult) {
        if (bindingResult.hasErrors()) {
            throw new ValidationException(bindingResult);
        }

        return postService.writePost(postForm, getUser(jwt));
    }

    @GetMapping(value={"/post/{id}"})
    public Post findPost(@PathVariable String id) {
        return getPost(id);
    }

    @GetMapping(value={"/post/{id}/comments"})
    public List<Comment> findComments(@PathVariable String id) {
        return commentService.findAll(getPost(id).getId());
    }

    @PostMapping(value={"/post/{id}/comments"})
    public Comment writeComment(@PathVariable String id, @Valid @RequestBody Comment comment, @RequestParam String jwt,
                              BindingResult bindingResult) {
        if (bindingResult.hasErrors()) {
            throw new ValidationException(bindingResult);
        }

        comment.setUser(getUser(jwt));
        comment.setPost(getPost(id));

        return commentService.writeComment(comment);
    }

    private Post getPost(String id) {
        try {
            Post post = postService.findById(Long.parseLong(id));

            if (post != null) {
                return post;
            }
        } catch (NumberFormatException ignored) {}

        throw new NoSuchResourceException();
    }

    private User getUser(String jwt) {
        User user = jwtService.find(jwt);

        if (user == null) {
            throw new UnauthorizedException();
        }

        return user;
    }
}
