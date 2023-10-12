package ru.itmo.wp.util;

import com.google.gson.stream.JsonReader;
import com.google.gson.stream.JsonToken;

import javax.servlet.http.HttpSession;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.*;

public class MessageUtils {
    public static class MessageStorage {
        private final List<Message> data = new ArrayList<>();

        public void post(final String user, final String text) throws IllegalAccessException {
            if (user == null) {
                throw new IllegalAccessException("Unauthorized message.");
            }

            synchronized (data) {
                data.add(new Message(user, text));
            }
        }

        public List<Message> get() {
            return data;
        }
    }

    public static class Message {
        public String user;
        public String text;

        public Message(final String user, final String text) {
            this.user = user;
            this.text = text;
        }
    }

    public static String getUser(final HttpSession session) {
        return (String) session.getAttribute("user");
    }

    public static String postUser(final HttpSession session, final String user) {
        if (user != null) {
            session.setAttribute("user", user);
        }

        return getUser(session);
    }
}
