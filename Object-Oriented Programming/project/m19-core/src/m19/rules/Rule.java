package m19.rules;

import java.io.Serializable;

import m19.Work;
import m19.User;
import m19.exceptions.RuleVerificationException;

public interface Rule extends Serializable {

    public void verify(Work work, User user) throws RuleVerificationException;
}