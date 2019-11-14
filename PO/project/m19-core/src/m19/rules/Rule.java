package m19.rules;

import java.io.Serializable;

import m19.works.*;
import m19.users.*;

public interface Rule extends Serializable {

    public boolean verify(Work work, User user);
}