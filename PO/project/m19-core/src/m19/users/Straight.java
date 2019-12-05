package m19.users;

import java.util.List;

import m19.works.Work;
import m19.requests.Request;

public class Straight extends Classification {

    private static final long serialVersionUID = 201912050023L;

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
        return 5;
    }

    public void update(List<Request> requests) {
        if (requests.size() >= 3) {
            int faults = 0;
            for (int i = requests.size() - 3; i < requests.size(); i++) {
                Request request = requests.get(i);
                if (request.getStatus()) {
                    faults++;
                }
            }
            if (faults == 3) {
                _user.setClassification(new Faulty(_user));
            }
        }
    }

    @Override
    public String toString() {
        return "CUMPRIDOR";
    }

}