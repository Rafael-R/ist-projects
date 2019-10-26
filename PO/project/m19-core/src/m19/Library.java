package m19;

import java.io.IOException;
import java.io.Serializable;
import java.util.Map;
import java.util.TreeMap;

import m19.users.*;
import m19.works.*;
import m19.exceptions.*;

/**
 * Class that represents the library as a whole.
 */
public class Library implements Serializable {

	/** Serial number for serialization. */
	private static final long serialVersionUID = 201901101348L;

	private int _userId = 0;
	private int _workId = 0;
	private int _date = 0;
	private Map<Integer, User> _users = new TreeMap<Integer, User>();

	

	// FIXME define contructor(s)

	public int getLastUserId() {
		return _userId;
	}

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

	public int registerUser(String name, String email) {
		int id = _userId++;
		User user = new User(id, name, email);
		_users.put(id, user);
		return id;
	}

	public User getUser(int id) {
		return _users.get(id);
	}

}
