package m19.rules;

import java.io.Serializable;

import m19.works.Work;
import m19.users.User;

public interface Rule extends Serializable {

    public boolean verify(Work work, User user);
}