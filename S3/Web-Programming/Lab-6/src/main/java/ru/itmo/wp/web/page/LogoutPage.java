package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Event;
import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

@SuppressWarnings({ "unused" })
public class LogoutPage extends Page {
    @Override
    protected void action(HttpServletRequest request, Map<String, Object> view) {
        getEventService().save(getUser(), Event.Type.LOGOUT);
        setUser(null);

        setMessage("Good bye. Hope to see you soon!");
        throw new RedirectException("/index");
    }
}
