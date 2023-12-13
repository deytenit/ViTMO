package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.web.annotation.Json;
import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

@SuppressWarnings({ "unused" })
public class ArticlePage extends Page {
    @Override
    protected void action(HttpServletRequest request, Map<String, Object> view) {
        User user = getUser();

        if (user == null) {
            setMessage("Authorize to post articles!");
            throw new RedirectException("/index");
        }
    }

    @Json
    private void create(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
        User user = getUser();

        Article article = new Article();
        article.setTitle(request.getParameter("title"));
        article.setText(request.getParameter("text"));
        article.setUserId(user.getId());
        article.setHidden(Boolean.parseBoolean(request.getParameter("hidden")));

        getArticleService().validateArticle(article);
        getArticleService().create(article);

        setMessage("New article has been posted!");

        throw new RedirectException("/index");
    }
}
