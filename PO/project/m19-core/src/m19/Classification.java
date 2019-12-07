package m19;

import java.io.Serializable;
import java.util.List;

import m19.Work;
import m19.Request;

public abstract class Classification implements Serializable {

    private static final long serialVersionUID = 201911281653L;

    protected User _user;

    public Classification(User user) {
        _user = user;
    }

    public abstract int maxReturnDays(Work work);

    public abstract int maxRequests();

    public abstract void update(List<Request> requests);
}