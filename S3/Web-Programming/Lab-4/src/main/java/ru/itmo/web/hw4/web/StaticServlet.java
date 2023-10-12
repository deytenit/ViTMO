package ru.itmo.web.hw4.web;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;


public class StaticServlet extends HttpServlet {
    final static String DEV_STATIC_PATH = "/../../src/main/webapp";

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        final String uri = request.getRequestURI();

        File file = new File(getServletContext().getRealPath("."), DEV_STATIC_PATH + uri);

        if (!file.isFile()) {
            file = new File(getServletContext().getRealPath(uri));
        }

        if (file.isFile()) {
            response.setContentType(getServletContext().getMimeType(file.getName()));
            Files.copy(file.toPath(), response.getOutputStream());
        } else {
            response.setStatus(HttpServletResponse.SC_NOT_FOUND);
        }
    }
}
