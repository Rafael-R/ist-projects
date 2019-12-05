package m19.exceptions;

/**
 * Class encoding file opening failure.
 */
public class FineToPayException extends Exception {

	/** Serial number for serialization. */
	static final long serialVersionUID = 201912042057L;

	/** Bad file name. */
	private int _fine;

	/**
	 * @param name
	 */
	public FineToPayException(int fine) {
		_fine = fine;
	}

	/**
	 * @return the fine
	 */
	public int getFine() {
		return _fine;
	}

}
