package m19.exceptions;

/** Class encoding the failure of borrowing requests. */
public class RuleVerificationException extends Exception {

	/** Serial number for serialization. */
	static final long serialVersionUID = 200510291601L;

	/** User id. */
	int _idUser;

	/** Work id. */
	int _idWork;

	/** Index of failed rule. */
	int _ruleIndex = -1;

	/**
	 * @param idUser
	 * @param idWork
	 * @param ruleIndex
	 */
	public RuleVerificationException(int idUser, int idWork, int ruleIndex) {
		_idUser = idUser;
		_idWork = idWork;
		_ruleIndex = ruleIndex;
	}

	/**
	 * @return user id
	 */
	public int getUser() {
		return _idUser;
	}
	
	/**
	 * @return work id
	 */
	public int getWork() {
		return _idWork;
	}

	/**
	 * @return index of failed rule
	 */
	public int getRuleIndex() {
		return _ruleIndex;
	}

}