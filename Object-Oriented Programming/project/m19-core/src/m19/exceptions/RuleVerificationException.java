package m19.exceptions;

/** Class encoding the failure of borrowing requests. */
public class RuleVerificationException extends Exception {

	/** Serial number for serialization. */
	static final long serialVersionUID = 200510291601L;

	/** User id. */
	int _userId;

	/** Work id. */
	int _workId;

	/** Index of failed rule. */
	int _ruleIndex = -1;

	/**
	 * @param userId
	 * @param workId
	 * @param ruleIndex
	 */
	public RuleVerificationException(int userId, int workId, int ruleIndex) {
		_userId = userId;
		_workId = workId;
		_ruleIndex = ruleIndex;
	}

	/**
	 * @return user id
	 */
	public int getUser() {
		return _userId;
	}
	
	/**
	 * @return work id
	 */
	public int getWork() {
		return _workId;
	}

	/**
	 * @return index of failed rule
	 */
	public int getRuleIndex() {
		return _ruleIndex;
	}

}