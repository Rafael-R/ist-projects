package m19.users;

import java.io.Serializable;

import m19.works.*;

public class Straight implements Classification, Serializable {

    private static final long serialVersionUID = 201911281654L;

    private static final int MAX_REQUESTS = 5;

    public int maxReturnDays(Work work) {
        int copies = work.getCopies();
        if (copies == 1) {
            return 8;
        } else if (copies > 1 && copies <= 5) {
            return 15;
        } else {
            return 30;
        }
    }

    public int maxRequests() {
        return MAX_REQUESTS;
    }

    @Override
    public String toString() {
        return "CUMPRIDOR";
    }

}