package m19.exceptions;

/** Launched when user registration fails. */
public class InvalidUserDataException extends Exception {
	
	/** Serial number for serialization. */
	private static final long serialVersionUID = 201901101348L;

	/** Bad user's name. */
	private String _name;
	
	/** Bad user's email. */
	private String _email;

	/**
	 * @param name
	 * @param email
	 */
	public InvalidUserDataException(String name, String email) {
		_name = name;
		_email = email;
	}

	public String getName() {
		return _name;
	}

	public String getEmail() {
		return _email;
	}


}
