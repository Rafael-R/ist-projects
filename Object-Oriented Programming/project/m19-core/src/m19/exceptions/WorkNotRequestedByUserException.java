package m19.exceptions;

/** Class encoding return failure. */
public class WorkNotRequestedByUserException extends Exception {
	
	/** Serial number for serialization. */
	static final long serialVersionUID = 200510291601L;

	/** Bad work id. */
    private int _workId;
    
	/** Bad user id. */
	private int _userId;

	/**
	 * @param workId
	 * @param userId
	 */
	public WorkNotRequestedByUserException(int workId, int userId) {
		_workId = workId;
		_userId = userId;
    }
     
    /**
     * @return work id
     */
    public int getWork() {
        return _workId;
    }
    
    /**
     * @return user id
     */
    public int getUser() {
        return _userId;
    }
}