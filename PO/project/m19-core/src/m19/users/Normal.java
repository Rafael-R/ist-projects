package m19.users;

import java.util.List;

import m19.works.Work;
import m19.requests.Request;

public class Normal extends Classification {

    private static final long serialVersionUID = 201912050023L;

    public Normal(User user) {
        super(user);
    }

    public int maxReturnDays(Work work) {
        int copies = work.getCopies();
        if (copies == 1) {
            return 3;
        } else if (copies <= 5) {
            return 8;
        } else {
            return 15;
        }
    }

    public int maxRequests() {
        return 3;
    }

    public void update(List<Request> requests) {
        if (requests.size() >= 5) {
            int counter = 0, faults = 0;
            for (int i = requests.size() - 5; i < requests.size(); i++) {
                Request request = requests.get(i);
                if (request.getFine() == 0 && request.isReturned()) {
                    counter++;
                }
                if (request.getFine() > 0 && i >= requests.size() - 3) {
                    faults++;
                }
            }
            if (counter == 5) {
                _user.setClassification(new Straight(_user));
            } else if (faults == 3) {
                _user.setClassification(new Faulty(_user));
            }
        }
    }

    @Override
    public String toString() {
        return "NORMAL";
    }

}