package m19;

import java.io.IOException;
import java.io.Serializable;
import m19.exceptions.BadEntrySpecificationException;

/**
 * Class that represents the library as a whole.
 */
public class Library implements Serializable {

	/** Serial number for serialization. */
	private static final long serialVersionUID = 201901101348L;

	private int _date = 0;

	

	// FIXME define contructor(s)

	// FIXME define methods

	/**
	 * Read the text input file at the beginning of the program and populates the
	 * instances of the various possible types (books, DVDs, users).
	 * 
	 * @param filename
	 *          name of the file to load
	 * @throws BadEntrySpecificationException
	 * @throws IOException
	 */
	void importFile(String filename) throws BadEntrySpecificationException, IOException {
		// FIXME implement method
	}


	// Date functions

	public int getDate() {
		return _date;
	}

	public void advanceDate(int daysToAdvance) {
		_date += daysToAdvance;
	}


	// User functions

	public void registerUser(Strig name, String email) {

	}

}
