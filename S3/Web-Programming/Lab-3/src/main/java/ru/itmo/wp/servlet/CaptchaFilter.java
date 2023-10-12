package ru.itmo.wp.servlet;

import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.http.HttpFilter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.Base64;
import java.util.concurrent.ThreadLocalRandom;

import static ru.itmo.wp.util.ImageUtils.toPng;

public class CaptchaFilter extends HttpFilter {
    private static final String TEMPLATE = "<!DOCTYPE html>\n" +
            "<html lang=\"en\">\n" +
            "<head>\n" +
            "    <meta charset=\"UTF-8\">\n" +
            "    <title>Codeforces</title>\n" +
            "</head>\n" +
            "<body>\n" +
            "    <form style=\"width: fit-content; margin: auto\">\n" +
            "        <img src=\"data:image/png;base64,${encodedImg}\" alt=\"captcha\">\n" +
            "        <br/>\n" +
            "        <label for=\"captcha_input\">Write text from the image:</label>\n" +
            "        <input type=\"text\" id=\"captcha_input\" name=\"captcha_attempt\">\n" +
            "        <br/>\n" +
            "        <input type=\"submit\">\n" +
            "    </form>\n" +
            "</body>\n" +
            "</html>";

    @Override
    protected void doFilter(HttpServletRequest request, HttpServletResponse response, FilterChain chain) throws IOException, ServletException {
        final boolean verify = request.getSession().getAttribute("captcha_passed") != null;

        if (verify) {
            chain.doFilter(request, response);
            return;
        }

        final String attempt = request.getParameter("captcha_attempt");
        String valid = (String) request.getSession().getAttribute("captcha_valid");

        if (attempt != null && attempt.equals(valid)) {
            request.getSession().setAttribute("captcha_passed", true);
            chain.doFilter(request, response);
            return;
        }

        if (valid == null || attempt != null) {
            valid = String.valueOf(ThreadLocalRandom.current().nextInt(100, 1000));
            request.getSession().setAttribute("captcha_valid", valid);
        }

        final String encodedImg = new String(Base64.getEncoder().encode(toPng(valid)));

        response.getWriter().write(TEMPLATE.replace("${encodedImg}", encodedImg));
        response.getWriter().flush();
    }
}
