package m19.rules;

import m19.works.*;
import m19.users.*;

public interface Rule {

    public boolean verify(Work work, User user);
}