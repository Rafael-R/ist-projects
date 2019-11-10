package m19.exceptions;

/** Class encoding return failure. */
public class WorkNotRequestedByUserException extends Exception {
	
	/** Serial number for serialization. */
	static final long serialVersionUID = 200510291601L;

	/** Bad work id. */
    private int _idWork;
    
	/** Bad user id. */
	private int _idUser;

	/**
	 * @param idWork
	 * @param idUser
	 */
	public WorkNotRequestedByUserException(int idWork, int idUser) {
		_idWork = idWork;
		_idUser = idUser;
    }
     
    /**
     * @return work id
     */
    public int getWork() {
        return _idWork;
    }
    
    /**
     * @return user id
     */
    public int getUser() {
        return _idUser;
    }
}