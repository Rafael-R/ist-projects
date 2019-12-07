package m19.exceptions;

/** Launched when import file has unknown data. */
public class InvalidDataException extends Exception {
	
	/** Serial number for serialization. */
	private static final long serialVersionUID = 201901101348L;

	/** Invalid data. */
	private String _data;
	private String _name;
	private String _email;

	/**
	 * @param data
	 */
	public InvalidDataException(String data) {
		_data = data;
	}

	public InvalidDataException(String name, String email) {
		_name = name;
		_email = email;
	}

	public String getData() {
		return _data;
	}

	public String getName() {
		return _name;
	}

	public String getEmail() {
		return _email;
	}

}