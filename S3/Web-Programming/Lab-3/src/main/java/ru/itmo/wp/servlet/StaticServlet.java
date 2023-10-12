package ru.itmo.wp.servlet;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;

public class StaticServlet extends HttpServlet {
    final static String DEV_STATIC_PATH = "/../../../src/main/webapp/static";

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        final String[] uris = request.getRequestURI().replaceAll("\\+", "+/").split("\\+");
        List<File> files = new ArrayList<>();

        for (String uri : uris) {
            File file = new File(getServletContext().getRealPath("/static"), DEV_STATIC_PATH + uri);

            if (!file.isFile()) {
                file = new File(getServletContext().getRealPath("/static" + uri));
            }

            if (!file.isFile()) {
                response.sendError(HttpServletResponse.SC_NOT_FOUND);
                return;
            }


            files.add(file);
        }


        if (files.isEmpty()) {
            return;
        }

        response.setContentType(getServletContext().getMimeType(files.get(0).getName()));

        for (File file : files) {
            try (OutputStream outputStream = response.getOutputStream()) {
                Files.copy(file.toPath(), outputStream);
            }
        }
    }
}
