package ru.itmo.wp.model.domain;

import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class User extends Entity implements SpecializedEntity {
    private String login;
    private String email;
    private boolean admin;

    public String getLogin() {
        return login;
    }

    public void setLogin(String login) {
        this.login = login;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public boolean isAdmin() {
        return admin;
    }

    public void setAdmin(final boolean admin) {
        this.admin = admin;
    }

    @Override
    public Map<String, Object> getSpecializedEntries() {
        return Map.of(
                "login", login, "email", email, "admin", admin
        );
    }

    @Override
    public Map<String, Object> getEntries() {
        return Stream.concat(super.getEntries().entrySet().stream(), getSpecializedEntries().entrySet().stream())
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    }
}
