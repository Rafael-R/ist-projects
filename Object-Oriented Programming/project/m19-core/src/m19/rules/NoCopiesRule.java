package m19.rules;

import m19.Work;
import m19.User;
import m19.exceptions.RuleVerificationException;

public class NoCopiesRule implements Rule {

    private static final long serialVersionUID = 201911161601L;

    private static final int INDEX = 3;

    @Override
    public void verify(Work work, User user) throws RuleVerificationException {
        if (!work.isAvailable()) {
            throw new RuleVerificationException(user.getId(), work.getId(), INDEX);
        }
    }
    
}