package ru.itmo.wp.service;

import org.springframework.stereotype.Service;
import ru.itmo.wp.domain.Post;
import ru.itmo.wp.domain.User;
import ru.itmo.wp.form.UserCredentialsRegister;
import ru.itmo.wp.form.WritePostForm;
import ru.itmo.wp.repository.PostRepository;

import java.util.List;
import java.util.stream.Collectors;

@Service
public class PostService {
    private final PostRepository postRepository;

    public PostService(PostRepository postRepository) {
        this.postRepository = postRepository;
    }

    public Post findById(long id) {
        return postRepository.findById(id).orElse(null);
    }

    public List<Post> findAll() {
        return postRepository.findAllByOrderByCreationTimeDesc();
    }

    public Post writePost(WritePostForm form, User author) {
        Post post = new Post();
        post.setTitle(form.getTitle());
        post.setText(form.getText());
        post.setUser(author);

        this.postRepository.save(post);

        return post;
    }
}
