package m19.rules;

import m19.works.*;
import m19.users.*;

public interface Rule {

    boolean verify(Work work, User user);
}