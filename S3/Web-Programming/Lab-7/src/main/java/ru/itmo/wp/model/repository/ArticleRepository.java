package ru.itmo.wp.model.repository;

import ru.itmo.wp.model.domain.Article;

import java.util.List;

public interface ArticleRepository {
    Article find(long id);
    List<Article> findAllByUserId(long userId);
    List<Article> findAll();
    void update(Article article);
    void save(Article article);
}
