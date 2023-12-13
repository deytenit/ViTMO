package ru.itmo.wp.model.repository.impl;

import ru.itmo.wp.model.domain.Talk;
import ru.itmo.wp.model.repository.TalkRepository;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.Collections;
import java.util.List;

public class TalkRepositoryImpl extends RepositoryImpl<Talk> implements TalkRepository {
    public TalkRepositoryImpl() {
        super("Talk");
    }

    @Override
    public Talk find(long id) {
        return super.findBy("id", id);
    }

    @Override
    public List<Talk> findAllByUserId(long userId) {
        return super.findAllWhere("? in (sourceUserId, targetUserId)", Collections.singletonList(userId));
    }

    @Override
    public void save(Talk talk) {
        super.save(talk, talk.getSpecializedEntries());
    }

    @Override
    protected Talk toEntity(ResultSetMetaData metaData, ResultSet resultSet) throws SQLException {
        if (!resultSet.next()) {
            return null;
        }

        Talk talk = new Talk();
        for (int i = 1; i <= metaData.getColumnCount(); i++) {
            switch (metaData.getColumnName(i)) {
                case "id":
                    talk.setId(resultSet.getLong(i));
                    break;
                case "sourceUserId":
                    talk.setSourceUserId(resultSet.getLong(i));
                    break;
                case "targetUserId":
                    talk.setTargetUserId(resultSet.getLong(i));
                    break;
                case "text":
                    talk.setText(resultSet.getString(i));
                    break;
                case "creationTime":
                    talk.setCreationTime(resultSet.getTimestamp(i));
                    break;
                default:
                    // No operations.
            }
        }

        return talk;
    }
}
