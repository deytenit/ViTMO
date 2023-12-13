package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.web.annotation.Json;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

@SuppressWarnings({ "unused" })
public class UsersPage extends Page {
    @Json
    private void findAll(HttpServletRequest request, Map<String, Object> view) {
        view.put("users", getUserService().findAll());
    }

    @Json
    private void findUser(HttpServletRequest request, Map<String, Object> view) {
        final long userId = Long.parseLong(request.getParameter("userId"));

        view.put("user", getUserService().findById(userId));
    }

    private void updateUserAdmin(final HttpServletRequest request, final Map<String, Object> view) throws ValidationException {
        User user = getUser();

        long userId = Long.parseLong(request.getParameter("id"));
        boolean admin = Boolean.parseBoolean(request.getParameter("admin"));

        if (!user.isAdmin()) {
            throw new ValidationException("Only admin can delegate admins");
        }
        if (user.getId() == userId) {
            throw new ValidationException("You can't revoke your admin rights");
        }

        getUserService().updateAdmin(userId, admin);
        view.put("admin", getUserService().findById(userId).isAdmin());
    }
}
