package m19.users;

import java.util.List;

import m19.users.*;
import m19.works.*;
import m19.requests.*;

public class Straight extends Classification {

    public Straight(User user) {
        super(user);
    }

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
        return 5;
    }

    public void update() {
        List<Request> requests = _user.getRequests();
        int faults = 0;
        for (int i = requests.size() - 3; i < requests.size(); i++) {
            Request request = requests.get(i);
            if (request.getFine() > 0) {
                faults++;
            }
        }
        if (faults == 3) {
            _user.setClassification(new Faulty(_user));
        }
    }

    @Override
    public String toString() {
        return "CUMPRIDOR";
    }

}