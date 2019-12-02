package m19.users;

import java.io.Serializable;

import m19.works.*;

public class Faulty implements Classification, Serializable {

    private static final long serialVersionUID = 201911281653L;

    private static final int MAX_REQUESTS = 1;

    public int maxReturnDays(Work work) {
        return 2;
    }

    public int maxRequests() {
        return MAX_REQUESTS;
    }
    
    @Override
    public String toString() {
        return "FALTOSO";
    }

}