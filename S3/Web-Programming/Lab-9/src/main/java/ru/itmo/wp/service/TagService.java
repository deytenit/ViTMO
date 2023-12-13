package ru.itmo.wp.service;

import org.springframework.stereotype.Service;
import ru.itmo.wp.domain.Role;
import ru.itmo.wp.domain.Tag;
import ru.itmo.wp.domain.User;
import ru.itmo.wp.form.UserCredentials;
import ru.itmo.wp.repository.RoleRepository;
import ru.itmo.wp.repository.TagRepository;
import ru.itmo.wp.repository.UserRepository;

import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

@Service
public class TagService {
    private final TagRepository tagRepository;

    public TagService(TagRepository tagRepository) {
        this.tagRepository = tagRepository;
    }

    public Tag findByName(String name) {
        return tagRepository.findByName(name);
    }

    public boolean existsByName(String name) {
        return tagRepository.existsByName(name);
    }

    public Set<Tag> generate(String tags) {
        return Arrays.stream(tags.split("\\s+"))
                .map(x -> existsByName(x) ? findByName(x) : new Tag(x.toLowerCase())).collect(Collectors.toSet());
    }
}
