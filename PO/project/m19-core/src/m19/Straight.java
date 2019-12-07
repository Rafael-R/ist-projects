package m19;

import java.util.List;

import m19.Work;
import m19.Request;

public class Straight extends Classification {

    private static final long serialVersionUID = 201912050023L;

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
        if (requests.size() >= 3) {
            int counter = 0;
            for (int i = requests.size() - 3; i < requests.size(); i++) {
                Request request = requests.get(i);
                if (request.getStatus()) {
                    counter++;
                }
            }
            if (counter == 3) {
                _user.setClassification(new Faulty(_user));
            }
        }
    }

    @Override
    public String toString() {
        return "CUMPRIDOR";
    }

}