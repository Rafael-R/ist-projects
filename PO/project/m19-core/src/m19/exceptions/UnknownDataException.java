package m19.exceptions;

/** Launched when import file has unknown data. */
public class UnknownDataException extends Exception {
	
	/** Serial number for serialization. */
	private static final long serialVersionUID = 201901101348L;

	/**
	 * Unknown data.
	 */
	private String _data;

	/**
	 * @param data
	 */
	public UnknownDataException(String data) {
		_data = data;
	}

	public String getData() {
		return _data;
	}

}