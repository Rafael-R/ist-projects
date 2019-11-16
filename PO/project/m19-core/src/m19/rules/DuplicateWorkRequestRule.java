package m19.rules;

import m19.works.*;
import m19.users.*;

public class DuplicateWorkRequestRule implements Rule {

    private static final long serialVersionUID = 201911161602L;

    @Override
    public boolean verify(Work work, User user) {
        return true;
    }

}