package m19.rules;

import m19.works.Work;
import m19.users.User;
import m19.exceptions.RuleVerificationException;

public class ReferenceWorksRule implements Rule {

    private static final long serialVersionUID = 201911161601L;

    private static final int INDEX = 5;

    @Override
    public void verify(Work work, User user) throws RuleVerificationException {
        if (work.getCategory().equals("Referência")) {
            throw new RuleVerificationException(user.getId(), work.getId(), INDEX);
        }
    }
    
}