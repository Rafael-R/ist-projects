package m19;

import java.util.List;

public class Faulty extends Classification {

    private static final int MAX_REQUESTS = 1;

    public Faulty(User user) {
        super(user);
    }

    public int maxReturnDays(Work work) {
        int copies = work.getCopies();
        if (copies == 1) {
            return 2;
        } else if (copies <= 5) {
            return 2;
        } else {
            return 2;
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
                if (!request.getStatus()) {
                    counter++;
                }
            }
            if (counter == 3) {
                _user.setClassification(new Normal(_user));
            }
        }
    }
    
    @Override
    public String toString() {
        return "FALTOSO";
    }

}