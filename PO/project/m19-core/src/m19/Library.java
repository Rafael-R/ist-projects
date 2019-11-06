package m19;

import java.io.IOException;
import java.io.Serializable;
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;
import java.util.regex.Pattern;

import m19.users.*;
import m19.works.*;
import m19.exceptions.BadEntrySpecificationException;
import m19.exceptions.UnknownDataException;

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
	void importFile(String filename) throws IOException, BadEntrySpecificationException {
		BufferedReader reader = new BufferedReader(new FileReader(filename));
		String line;
  		while ((line = reader.readLine()) != null) {
			String[] fields = line.split(":");
			try {
				registerFromFields(fields);
			} catch (UnknownDataException e) {
				throw new BadEntrySpecificationException(e.getData());
			}
		} 
	}

	void registerFromFields(String[] fields) throws UnknownDataException {

		Pattern patternUser = Pattern.compile("^(USER)");
		Pattern patternWork = Pattern.compile("^(DVD|BOOK)");

		if (patternUser.matcher(fields[0]).matches()) {
			registerUser(fields[1], fields[2]);
		} else if (patternWork.matcher(fields[0]).matches()) {
			registerWork(fields);
		} else {
			throw new UnknownDataException(fields[0]);
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

	public int registerUser(String name, String email) {
		int id = _userId++;
		User user = new User(id, name, email);
		_users.put(id, user);
		return id;
	}

	public User fetchtUser(int id) {
		return _users.get(id);
	}

	public String getUserString(int id) {
		return _users.get(id).toString();
	}

	public String getUserNotifications(int id) {
		return _users.get(id).getNotifications();
	}

	public boolean getUserState(int id) {
		return _users.get(id).getState();
	}

	public void userPayFine(int id) {
		_users.get(id).payFine();
	}


	// Works functions

	public int getLastWorkId() {
		return _workId;
	}

	void registerWork(String[] fields) {
		int price = Integer.parseInt(fields[3]);
		int copies = Integer.parseInt(fields[6]);
		int id = _workId++;
		Work work;
		switch (fields[0]) {
			case "DVD":
				work = new Dvd(id, fields[1], fields[2], price, fields[4], fields[5], copies);
				break;
			case "BOOK":
				work = new Book(id, fields[1], fields[2], price, fields[4], fields[5], copies);
				break;
			default:
				break;
		}
		_works.put(id, work);
	}

	public Work fetchWork(int id) {
		return _works.get(id);
	}

	public String getWorkString(int id) {
		return _works.get(id).toString();
	}

	public boolean workHasTerm(int id, String term) {
		return _works.get(id).hasTerm(term);
	}

}
