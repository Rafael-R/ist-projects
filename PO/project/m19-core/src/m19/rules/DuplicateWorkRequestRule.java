package m19.rules;

import m19.works.Work;
import m19.users.User;

public class DuplicateWorkRequestRule implements Rule {

    private static final long serialVersionUID = 201911161602L;

    @Override
    public boolean verify(Work work, User user) {
        return user.hasRequested(work.getId()) == null;
    }

}