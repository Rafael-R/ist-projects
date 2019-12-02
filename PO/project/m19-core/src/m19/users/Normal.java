package m19.users;

import java.io.Serializable;

import m19.works.*;

public class Normal implements Classification, Serializable {

    private static final long serialVersionUID = 201911281653L;

    private static final int MAX_REQUESTS = 3;

    public int maxReturnDays(Work work) {
        int copies = work.getCopies();
        if (copies == 1) {
            return 3;
        } else if (copies > 1 && copies <= 5) {
            return 8;
        } else {
            return 5;
        }
    }

    public int maxRequests() {
        return MAX_REQUESTS;
    }

    @Override
    public String toString() {
        return "NORMAL";
    }

}