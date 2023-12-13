package ru.itmo.wp.model.repository.impl;

import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.repository.UserRepository;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class UserRepositoryImpl extends RepositoryImpl<User> implements UserRepository {
    public UserRepositoryImpl() {
        super("User");
    }

    @Override
    public User find(long id) {
        return super.findBy("id", id);
    }

    @Override
    public User findByLogin(String login) {
        return super.findBy("login", login);
    }


    @Override
    public User findByEmail(String email) {
        return super.findBy("email", email);
    }

    @Override
    public User findByLoginAndPasswordSha(String login, String passwordSha) {
        return super.findBy(Map.of("login", login, "passwordSha", passwordSha));
    }

    @Override
    public User findByEmailAndPasswordSha(String email, String passwordSha) {
        return super.findBy(Map.of("email", email, "passwordSha", passwordSha));
    }

    @Override
    public User findByLoginOrEmailAndPasswordSha(String loginOrEmail, String passwordSha) {
        return super.findWhere(
                "(email=? OR login=?) AND passwordSha=?",
                List.of(loginOrEmail, loginOrEmail, passwordSha)
        );
    }

    @Override
    public void save(User user, String passwordSha) {
        HashMap<String, Object> columns = new HashMap<>(user.getSpecializedEntries());
        columns.put("passwordSha", passwordSha);

        super.save(user, columns);
    }

    @Override
    public List<User> findAll() {
        return super.findAll();
    }

    @Override
    public long countAll() {
        return super.countAll();
    }

    @Override
    protected User toEntity(ResultSetMetaData metaData, ResultSet resultSet) throws SQLException {
        if (!resultSet.next()) {
            return null;
        }

        User user = new User();
        for (int i = 1; i <= metaData.getColumnCount(); i++) {
            switch (metaData.getColumnName(i)) {
                case "id":
                    user.setId(resultSet.getLong(i));
                    break;
                case "login":
                    user.setLogin(resultSet.getString(i));
                    break;
                case "email":
                    user.setEmail(resultSet.getString(i));
                    break;
                case "creationTime":
                    user.setCreationTime(resultSet.getTimestamp(i));
                    break;
                default:
                    // No operations.
            }
        }

        return user;
    }
}
