package m19.users;

import java.io.Serializable;

import m19.works.*;

public class Straight implements Classification, Serializable {

    private static final long serialVersionUID = 201911281654L;

    private static final int MAX_RETURN_DAYS = 8;
    private static final int MAX_REQUESTS = 5;

    public int maxReturnDays(Work work) {
        return MAX_RETURN_DAYS;
    }

    public int maxRequests() {
        return MAX_REQUESTS;
    }

    @Override
    public String toString() {
        return "CUMPRIDOR";
    }

}