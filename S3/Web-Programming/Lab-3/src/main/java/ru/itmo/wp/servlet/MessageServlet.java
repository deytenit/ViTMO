package ru.itmo.wp.servlet;

import com.google.gson.Gson;
import ru.itmo.wp.util.MessageUtils;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

import static ru.itmo.wp.util.MessageUtils.getUser;
import static ru.itmo.wp.util.MessageUtils.postUser;

public class MessageServlet extends HttpServlet {
    MessageUtils.MessageStorage storage = new MessageUtils.MessageStorage();

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {
        final String uri = request.getRequestURI();

        response.setCharacterEncoding(StandardCharsets.UTF_8.name());

        if (uri.startsWith("/message/auth")) {
            final String user = postUser(request.getSession(), request.getParameter("user"));

            if (user != null) {
                response.setContentType("application/json");
                response.getWriter().print("\"" + user + "\"");
                response.getWriter().flush();
            }
        }
        else if (uri.startsWith("/message/findAll")) {
            final String body = new Gson().toJson(storage.get());

            response.setContentType("application/json");
            response.getWriter().print(body);
            response.getWriter().flush();
        }
        else if (uri.startsWith("/message/add")) {
            final String text = request.getParameter("text");

            try {
                storage.post(getUser(request.getSession()), text);
            } catch (IllegalAccessException err) {
                response.sendError(HttpServletResponse.SC_UNAUTHORIZED);
            }
        }
        else {
            response.sendError(HttpServletResponse.SC_NOT_FOUND);
        }
    }
}
