package ru.itmo.wp.web.page;

import ru.itmo.wp.model.domain.Talk;
import ru.itmo.wp.model.domain.User;
import ru.itmo.wp.model.exception.ValidationException;
import ru.itmo.wp.web.exception.RedirectException;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;
import java.util.stream.Collectors;

@SuppressWarnings({ "unused" })
public class TalksPage extends Page {
    @Override
    protected void action(HttpServletRequest request, Map<String, Object> view) {
        User user = getUser();

        if (user == null) {
            setMessage("Authorize to talk with other users!");
            throw new RedirectException("/index");
        }

        view.put("talks", getTalkService().findAllByUserId(user.getId()));
        view.put("logins", getUserService().findAll().stream().collect(Collectors.toMap(x -> String.valueOf(x.getId()), User::getLogin)));
    }

    private void talk(HttpServletRequest request, Map<String, Object> view) throws ValidationException {
        User sourceUser = getUser();
        User targetUser = getUserService().findByLogin(request.getParameter("target"));

        Talk talk = new Talk();
        talk.setSourceUserId(sourceUser.getId());
        talk.setTargetUserId(targetUser.getId());
        talk.setText(request.getParameter("text"));

        getTalkService().validateTalk(talk);
        getTalkService().send(talk);
        throw new RedirectException("/talks");
    }
}
