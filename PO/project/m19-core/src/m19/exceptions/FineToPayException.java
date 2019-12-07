package m19.exceptions;

public class FineToPayException extends Exception {

	/** Serial number for serialization. */
	static final long serialVersionUID = 201912071610L;

	private int _userId;
	private int _fine;

	public FineToPayException(int userId, int fine) {
		_userId = userId;
		_fine = fine;
	}

	public int getUserId() {
		return _userId;
	}

	public int getFine() {
		return _fine;
	}

}
