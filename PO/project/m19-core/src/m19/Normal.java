package m19;

import java.util.List;

public class Normal extends Classification {

    private static final int MAX_REQUESTS = 3;

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
        return MAX_REQUESTS;
    }

    public void update(List<Request> requests) {
        if (requests.size() >= 3) {
            int counter = 0, faults = 0;
            for (int i = requests.size() - 3; i < requests.size(); i++) {
                Request request = requests.get(i);
                if (request.getStatus()) {
                    faults++;
                } else if (!request.getStatus()) {
                    counter++;
                }
            }
            if (counter == 3 && requests.size() >= 5) {
                for (int i = requests.size() - 5; i < requests.size() - 3; i++) {
                    Request request = requests.get(i);
                    if (!request.getStatus()) {
                        counter++;
                    }
                }
                if (counter == 5) {
                    _user.setClassification(new Straight(_user));
                }
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