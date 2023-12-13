package ru.itmo.wp.model.repository.impl;

import org.checkerframework.checker.units.qual.A;
import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.Event;
import ru.itmo.wp.model.repository.ArticleRepository;
import ru.itmo.wp.model.repository.EventRepository;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.List;

public class ArticleRepositoryImpl extends RepositoryImpl<Article> implements ArticleRepository {
    public ArticleRepositoryImpl() {
        super("Article");
    }

    @Override
    public Article find(final long id) {
        return super.findBy("id", id);
    }

    @Override
    public List<Article> findAllByUserId(final long userId) {
        return super.findAllWhere("?=userId", List.of(userId));
    }

    @Override
    public List<Article> findAll() {
        return super.findAllWhere("?=hidden", List.of(0));
    }

    @Override
    public void save(Article article) {
        super.save(article, article.getSpecializedEntries());
    }

    @Override
    public void update(Article article) {
        super.update(article, article.getSpecializedEntries());
    }

    @Override
    protected Article toEntity(ResultSetMetaData metaData, ResultSet resultSet) throws SQLException {
        if (!resultSet.next()) {
            return null;
        }

        Article article = new Article();
        for (int i = 1; i <= metaData.getColumnCount(); i++) {
            switch (metaData.getColumnName(i)) {
                case "id":
                    article.setId(resultSet.getLong(i));
                    break;
                case "userId":
                    article.setUserId(resultSet.getLong(i));
                    break;
                case "title":
                    article.setTitle(resultSet.getString(i));
                    break;
                case "text":
                    article.setText(resultSet.getString(i));
                    break;
                case "hidden":
                    article.setHidden(resultSet.getBoolean(i));
                    break;
                case "creationTime":
                    article.setCreationTime(resultSet.getTimestamp(i));
                    break;
                default:
                    // No operations.
            }
        }

        return article;
    }
}
