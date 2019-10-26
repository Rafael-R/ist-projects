package m19.users;

import m19.works.*;

public class Normal implements Classification {

    private static final int MAX_RETURN_DAYS = 5;
    private static final int MAX_REQUESTS = 3;

    public int maxReturnDays(Work work) {
        return MAX_RETURN_DAYS;
    }

    public int maxRequests() {
        return MAX_REQUESTS;
    }

    @Override
    public String toString() {
        return "NORMAL";
    }

}