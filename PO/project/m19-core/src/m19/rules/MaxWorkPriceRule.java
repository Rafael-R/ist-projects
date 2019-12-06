package m19.rules;

import m19.works.Work;
import m19.users.User;

public class MaxWorkPriceRule implements Rule {

    private static final long serialVersionUID = 201911161602L;

    @Override
    public boolean verify(Work work, User user) {
        if (work.getPrice() > 25) {
            if (user.getClassification().equals("CUMPRIDOR")) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }   

}