package ru.itmo.wp.model.service;

import ru.itmo.wp.model.domain.Talk;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.model.repository.TalkRepository;
import ru.itmo.wp.model.repository.UserRepository;
import ru.itmo.wp.model.repository.impl.TalkRepositoryImpl;
import ru.itmo.wp.model.repository.impl.UserRepositoryImpl;

import java.util.List;

public class TalkService {
    private final TalkRepository talkRepository = new TalkRepositoryImpl();
    private final UserRepository userRepository = new UserRepositoryImpl();

    public void validateTalk(Talk talk) throws ValidationException {
        User sourceUser = userRepository.find(talk.getSourceUserId());
        User targetUser = userRepository.find(talk.getTargetUserId());
        String text = talk.getText().trim();

        if (targetUser == null) {
            throw new ValidationException("Recipient user doesn't exist.");
        }
        if (sourceUser.getId() == targetUser.getId()) {
            throw new ValidationException("You can't talk to yourself.");
        }

        if (text.isEmpty()) {
            throw new ValidationException("Text can't be empty.");
        }
        if (text.length() > 255) {
            throw new ValidationException("Text can't be longer than 255 characters");
        }
    }

    public List<Talk> findAllByUserId(long userId) {
        return talkRepository.findAllByUserId(userId);
    }

    public void send(Talk talk) {
        talkRepository.save(talk);
    }
}