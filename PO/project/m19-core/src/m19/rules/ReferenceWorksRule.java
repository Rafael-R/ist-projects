package m19.rules;

import m19.works.Work;
import m19.users.User;

public class ReferenceWorksRule implements Rule {

    private static final long serialVersionUID = 201911161601L;

    @Override
    public boolean verify(Work work, User user) {
        return !work.getCategory().equals("Referência");
    }
    
}