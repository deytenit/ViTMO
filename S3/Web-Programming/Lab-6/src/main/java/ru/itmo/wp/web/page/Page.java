package ru.itmo.wp.web.page;

import com.google.common.base.Strings;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.service.EventService;
import ru.itmo.wp.model.service.TalkService;
import ru.itmo.wp.model.service.UserService;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

@SuppressWarnings({ "unused" })
public class Page {
    private final UserService userService = new UserService();
    private final EventService eventService = new EventService();
    private final TalkService talkService = new TalkService();
    private HttpServletRequest request = null;

    protected void before(HttpServletRequest currentRequest, Map<String, Object> view) {
        request = currentRequest;
        putUser(view);
        putMessage(view);
        view.put("userCount", userService.findCount());
    }

    protected void action(HttpServletRequest request, Map<String, Object> view) {
        // No operations.
    }

    protected void after(HttpServletRequest request, Map<String, Object> view) {}

    protected UserService getUserService() {
        return userService;
    }

    protected EventService getEventService() {
        return eventService;
    }

    protected TalkService getTalkService() {return talkService;}

    protected void setMessage(String message) {
        if (message == null) {
            request.getSession().removeAttribute("message");
        }

        request.getSession().setAttribute("message", message);
    }

    protected User getUser() {
        return (User) request.getSession().getAttribute("user");
    }

    protected void setUser(User user) {
        if (user == null) {
            request.getSession().removeAttribute("user");
        }

        request.getSession().setAttribute("user", user);
    }

    private void putUser(Map<String, Object> view) {
        User user = (User) request.getSession().getAttribute("user");
        if (user != null) {
            view.put("user", user);
        }
    }

    private void putMessage(Map<String, Object> view) {
        String message = (String) request.getSession().getAttribute("message");
        if (!Strings.isNullOrEmpty(message)) {
            view.put("message", message);
            request.getSession().removeAttribute("message");
        }
    }
}
