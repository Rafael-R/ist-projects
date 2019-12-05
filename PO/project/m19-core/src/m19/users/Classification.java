package m19.users;

import java.io.Serializable;

import m19.users.*;
import m19.works.*;
import m19.requests.*;

public abstract class Classification implements Serializable {

    private static final long serialVersionUID = 201911281653L;

    protected User _user;

    public Classification(User user) {
        _user = user;
    }

    public abstract int maxReturnDays(Work work);

    public abstract int maxRequests();

    public abstract void update();
}