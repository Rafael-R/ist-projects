package m19.users;

import java.util.List;

import m19.users.*;
import m19.works.*;
import m19.requests.*;

public class Faulty extends Classification {

    public Faulty(User user) {
        super(user);
    }

    public int maxReturnDays(Work work) {
        return 2;
    }

    public int maxRequests() {
        return 1;
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
        if (faults == 0) {
            _user.setClassification(new Normal(_user));
        }
    }
    
    @Override
    public String toString() {
        return "FALTOSO";
    }

}