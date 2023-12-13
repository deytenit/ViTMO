package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Article;

import javax.servlet.http.HttpServletRequest;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

@SuppressWarnings({ "unused" })
public class IndexPage extends Page {
    private void findAllArticlesWithLogins(HttpServletRequest request, Map<String, Object> view) {
        ArrayList<Map<String, Object>> result = new ArrayList<>();

        for (Article entry : getArticleService().findAll()) {
            HashMap<String, Object> tmp = new HashMap<>(entry.getEntries());
            tmp.put("userLogin", getUserService().findById(entry.getUserId()).getLogin());

            result.add(tmp);
        }

        view.put("result", result);
    }
}
