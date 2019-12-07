package m19.rules;

import m19.Work;
import m19.User;
import m19.exceptions.RuleVerificationException;

public class MaxWorkPriceRule implements Rule {

    private static final long serialVersionUID = 201911161602L;

    private static final int INDEX = 6;

    @Override
    public void verify(Work work, User user) throws RuleVerificationException {
        if (work.getPrice() > 25 && !user.getClassification().equals("CUMPRIDOR")) {
            throw new RuleVerificationException(user.getId(), work.getId(), INDEX);
        }
    }

}