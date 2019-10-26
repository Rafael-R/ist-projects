package m19.users;

import m19.works.*;

public class Faulty implements Classification {

    private static final int MAX_RETURN_DAYS = 2;
    private static final int MAX_REQUESTS = 2;

    public int maxReturnDays(Work work) {
        return MAX_RETURN_DAYS;
    }

    public int maxRequests() {
        return MAX_REQUESTS;
    }
    
    @Override
    public String toString() {
        return "FALTOSO";
    }

}