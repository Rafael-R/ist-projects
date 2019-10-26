package m19.rules;

import m19.works.*;
import m19.users.*;

public class MaxWorkPriceRule implements Rule {

    @Override
    public boolean verify(Work work, User user) {
        return true;
    }

}