package m19.users;

import java.util.List;

import m19.users.*;
import m19.works.*;
import m19.requests.*;

public class Normal extends Classification {

    public Normal(User user) {
        super(user);
    }

    public int maxReturnDays(Work work) {
        int copies = work.getCopies();
        if (copies == 1) {
            return 3;
        } else if (copies > 1 && copies <= 5) {
            return 8;
        } else {
            return 15;
        }
    }

    public int maxRequests() {
        return 3;
    }

    public void update() {
        List<Request> requests = _user.getRequests();
        int straight_faults = 0, faulty_faults = 0;
        for (int i = requests.size() - 5; i < requests.size(); i++) {
            Request request = requests.get(i);
            if (request.getFine() > 0) {
                straight_faults++;
            }
            if (request.getFine() > 0 && i > requests.size() - 3) {
                faulty_faults++;
            }
        }
        if (straight_faults == 0) {
            _user.setClassification(new Straight(_user));
        } else if (faulty_faults == 3) {
            _user.setClassification(new Faulty(_user));
        }
    }

    @Override
    public String toString() {
        return "NORMAL";
    }

}