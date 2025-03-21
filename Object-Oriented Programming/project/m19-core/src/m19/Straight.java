package m19;

import java.util.List;

public class Straight extends Classification {

    private static final int MAX_REQUESTS = 5;

    public Straight(User user) {
        super(user);
    }

    public int maxReturnDays(Work work) {
        int copies = work.getCopies();
        if (copies == 1) {
            return 8;
        } else if (copies <= 5) {
            return 15;
        } else {
            return 30;
        }
    }

    public int maxRequests() {
        return MAX_REQUESTS;
    }

    public void update(List<Request> requests) {
        int counter = 0;
        for (int i = requests.size() - 5; i < requests.size(); i++) {
            Request request = requests.get(i);
            if (request.isReturned() && request.getFine() > 0) {
                counter++;
            }
        }
        if (counter > 0) {
            _user.setClassification(new Normal(_user));
        }
    }

    @Override
    public String toString() {
        return "CUMPRIDOR";
    }

}