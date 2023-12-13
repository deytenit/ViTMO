package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Event;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.web.annotation.Json;
import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

@SuppressWarnings({ "unused" })
public class EnterPage extends Page {
    @Json
    private void enter(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
        String loginOrEmail = request.getParameter("loginOrEmail");
        String password = request.getParameter("password");

        getUserService().validateEnter(loginOrEmail, password);
        User user = getUserService().findByLoginOrEmailAndPassword(loginOrEmail, password);
        getEventService().save(user, Event.Type.ENTER);

        setUser(user);
        setMessage("Hello, " + user.getLogin());

        throw new RedirectException("/index");
    }
}
