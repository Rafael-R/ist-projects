package m19;

import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;

import m19.users.*;
import m19.works.*;
import m19.exceptions.BadEntrySpecificationException;

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
	private Map<Integer, Work> _works = new TreeMap<Integer, Work>();
	

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
		BufferedReader reader = new BufferedReader(new FileReader(filename));
  		String line; 
  		while ((line = reader.readLine()) != null) 
			String[] inputs = line.split(":");
			switch (inputs[0]) {
				case "USER":
					addUser(inputs[1], inputs[2])
					break;
				case "DVD":
					for (int i = 0; i < inputs[6]; i++) {
						addDvd(inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
					}
					break;
				case "BOOK":
					for (int i = 0; i < inputs[6]; i++) {
						addBook(inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
					}
					break;
				default:
					break;
			}
		} 
	}


	// Date functions

	public int getDate() {
		return _date;
	}

	public void advanceDate(int daysToAdvance) {
		_date += daysToAdvance;
	}


	// Users functions

	public int getLastUserId() {
		return _userId;
	}

	public int addUser(String name, String email) {
		int id = _userId++;
		User user = new User(id, name, email);
		_users.put(id, user);
		return id;
	}

	public User getUser(int id) {
		return _users.get(id);
	}

	public String getUserString(int id) {
		return _users.get(id).toString();
	}


	// Works functions

	public int getLastWorkId() {
		return _workId;
	}

	public void addDvd(String title, String director, int price,
					   String category, String igac, int copies) {
		int id = _workId++;
		Work work = new Dvd(title, director, price, category, igac, copies);
		_works.put(id, work);
	}

	public void addBook(String title, String author, int price,
						String category, String isbn, int copies) {
		int id = _workId++;
		Work work = new Book(id, title, author, price, category, isbn, copies);
		_works.put(id, work);
	}

	public Work getWork(int id) {
		return _works.get(id);
	}

	public String getWorkString(int id) {
		return _works.get(id).toString();
	}

}
