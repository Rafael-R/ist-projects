package m19;

import java.io.Serializable;
import java.util.List;

public abstract class Classification implements Serializable {

    private static final long serialVersionUID = 201912071640L;

    protected User _user;

    public Classification(User user) {
        _user = user;
    }

    public abstract int maxReturnDays(Work work);

    public abstract int maxRequests();

    public abstract void update(List<Request> requests);
}