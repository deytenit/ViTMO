package ru.itmo.wp.model.repository.impl;

import ru.itmo.wp.model.database.DatabaseUtils;
import ru.itmo.wp.model.domain.Entity;
import ru.itmo.wp.model.exception.RepositoryException;

import javax.sql.DataSource;
import java.sql.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public abstract class RepositoryImpl<T extends Entity> {
    private final DataSource DATA_SOURCE = DatabaseUtils.getDataSource();
    private final String tableName;

    protected RepositoryImpl(String tableName) {
        this.tableName = tableName;
    }

    private static String stringValueOf(Object object) {
        String result = String.valueOf(object);

        if (result.equals("true") || result.equals("false")) {
            return result.equals("true") ? "1" : "0";
        }

        return result;
    }

    private static void setStatementColumns(PreparedStatement statement, Collection<Object> values) throws SQLException {
        int columnIndex = 1;
        for (Object value : values) {
            statement.setString(columnIndex++, stringValueOf(value));
        }
    }

    public T findBy(String key, Object value) {
        return findBy(Collections.singletonMap(key, value));
    }

    public T findBy(Map<String, Object> columns) {
        return findWhere(columns.keySet().stream().map(x -> x + "=?").collect(Collectors.joining(" AND ")), columns.values());
    }

    public T findWhere(String whereStatement, Collection<Object> objects) {
        String statementString = "SELECT * FROM %s WHERE %s".formatted(tableName, whereStatement.isEmpty() ? "TRUE" : whereStatement);

        try (Connection connection = DATA_SOURCE.getConnection()) {
            try (PreparedStatement statement = connection.prepareStatement(statementString)) {

                setStatementColumns(statement, objects);

                try (ResultSet resultSet = statement.executeQuery()) {
                    return toEntity(statement.getMetaData(), resultSet);
                }
            }
        } catch (SQLException e) {
            throw new RepositoryException("Can't find %s.".formatted(tableName), e);
        }
    }

    public List<T> findAll() {
        return findAllWhere("", Collections.emptyList());
    }

    public List<T> findAllWhere(String whereStatement, Collection<Object> objects) {
        List<T> entities = new ArrayList<>();
        String statementString = "SELECT * FROM %s WHERE %s ORDER BY creationTime DESC".formatted(
                tableName,
                whereStatement.isEmpty() ? "TRUE" : whereStatement
        );

        try (Connection connection = DATA_SOURCE.getConnection()) {
            try (PreparedStatement statement = connection.prepareStatement(statementString)) {
                setStatementColumns(statement, objects);

                try (ResultSet resultSet = statement.executeQuery()) {
                    T entity;
                    while ((entity = toEntity(statement.getMetaData(), resultSet)) != null) {
                        entities.add(entity);
                    }
                }
            }
        } catch (SQLException e) {
            throw new RepositoryException("Can't find %s.".formatted(tableName), e);
        }

        return entities;
    }

    public long countAll() {
        String statementString = "SELECT COUNT(*) FROM %s".formatted(tableName);

        try (Connection connection = DATA_SOURCE.getConnection()) {
            try (PreparedStatement statement = connection.prepareStatement(statementString)) {
                try (ResultSet resultSet = statement.executeQuery()) {
                    if (!resultSet.next()) {
                        return 0;
                    }

                    return resultSet.getLong(1);
                }
            }
        } catch (SQLException e) {
            throw new RepositoryException("Can't count %s.".formatted(tableName), e);
        }
    }

    public void update(T entity, Map<String, Object> columns) {
        String statementString = "UPDATE %s SET %s WHERE id=?".formatted(
                tableName,
                columns.keySet().stream().map(x -> x + "=?").collect(Collectors.joining(", "))
        );

        try (Connection connection = DATA_SOURCE.getConnection()) {
            try (PreparedStatement statement = connection.prepareStatement(statementString)) {
                setStatementColumns(statement, Stream.concat(columns.values().stream(), Stream.of(entity.getId())).collect(Collectors.toList()));

                if (statement.executeUpdate() != 1) {
                    throw new RepositoryException("Can't update %s.".formatted(tableName));
                }
            }
        } catch (SQLException e) {
            throw new RepositoryException("Can't count %s.".formatted(tableName), e);
        }
    }

    public void save(T entity, Map<String, Object> columns) {
        try (Connection connection = DATA_SOURCE.getConnection()) {
            String statementString = "INSERT INTO %s (%s, `creationTime`) VALUES (%s, NOW())".formatted(
                    tableName,
                    columns.keySet().stream().map(x -> '`' + x + '`').collect(Collectors.joining(", ")),
                    String.join(", ", Collections.nCopies(columns.keySet().size(), "?"))
            );

            try (PreparedStatement statement = connection.prepareStatement(statementString, Statement.RETURN_GENERATED_KEYS)) {

                setStatementColumns(statement, columns.values());

                if (statement.executeUpdate() != 1) {
                    throw new RepositoryException("Can't save %s.".formatted(tableName));
                } else {
                    ResultSet generatedKeys = statement.getGeneratedKeys();
                    if (generatedKeys.next()) {
                        entity.setId(generatedKeys.getLong(1));
                        entity.setCreationTime(findBy("id", entity.getId()).getCreationTime());
                    } else {
                        throw new RepositoryException("Can't save %s [no autogenerated fields].".formatted(tableName));
                    }
                }
            }
        } catch (SQLException e) {
            throw new RepositoryException("Can't save %s.".formatted(tableName), e);
        }
    }

    protected abstract T toEntity(ResultSetMetaData metaData, ResultSet resultSet) throws SQLException;
}
