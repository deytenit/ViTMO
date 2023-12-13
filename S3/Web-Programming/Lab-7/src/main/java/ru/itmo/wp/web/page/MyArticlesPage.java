package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Article;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

public class MyArticlesPage extends Page {
    @Override
    protected void action(final HttpServletRequest request, final Map<String, Object> view) {
        User user = getUser();

        if (user == null) {
            setMessage("Authorize to manage your articles!");
            throw new RedirectException("/index");
        }

        view.put("articles", getArticleService().findAllByUserId(user.getId()));
    }

    private void updateArticleHidden(final HttpServletRequest request, final Map<String, Object> view) throws ValidationException {
        User user = getUser();

        long articleId = Long.parseLong(request.getParameter("id"));
        boolean hidden = Boolean.parseBoolean(request.getParameter("hidden"));

        getArticleService().validateUpdate(articleId, user.getId());
        getArticleService().updateHidden(articleId, hidden);

        view.put("hidden", getArticleService().findById(articleId).isHidden());
    }
}
