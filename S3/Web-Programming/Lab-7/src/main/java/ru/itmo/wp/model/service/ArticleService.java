package ru.itmo.wp.model.service;

import org.checkerframework.checker.units.qual.A;
import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.model.repository.ArticleRepository;
import ru.itmo.wp.model.repository.UserRepository;
import ru.itmo.wp.model.repository.impl.ArticleRepositoryImpl;
import ru.itmo.wp.model.repository.impl.UserRepositoryImpl;

import java.util.List;

public class ArticleService {
    private final ArticleRepository articleRepository = new ArticleRepositoryImpl();
    private final UserRepository userRepository = new UserRepositoryImpl();

    public void validateArticle(Article article) throws ValidationException {
        String title = article.getTitle().trim();
        String text = article.getText().trim();


        if (title.isEmpty()) {
            throw new ValidationException("Title can't be empty.");
        }
        if (title.length() > 255) {
            throw new ValidationException("Title can't be longer than 255 characters");
        }

        if (text.isEmpty()) {
            throw new ValidationException("Text can't be empty.");
        }
    }

    public void validateUpdate(long id, long userId) throws ValidationException {
        Article article = articleRepository.find(id);
        User user = userRepository.find(userId);

        if (article == null) {
            throw new ValidationException("Article does not exist");
        }

        if (user == null) {
            throw new ValidationException("User does not exists");
        }

        if (article.getUserId() != userId && !user.isAdmin()) {
            throw new ValidationException("Only author or admin can update this article");
        }
    }

    public List<Article> findAll() {
        return articleRepository.findAll();
    }

    public List<Article> findAllByUserId(long userId) {
        return articleRepository.findAllByUserId(userId);
    }

    public void updateHidden(long id, boolean hidden) {
        Article article = findById(id);
        article.setHidden(hidden);

        articleRepository.update(article);
    }

    public void create(Article article) {
        articleRepository.save(article);
    }

    public Article findById(long id) {
        return articleRepository.find(id);
    }
}