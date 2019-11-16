package m19;

import java.io.IOException;
import java.io.Serializable;
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Map;
import java.util.TreeMap;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.regex.Pattern;
import java.util.Collections;

import m19.users.*;
import m19.works.*;
import m19.exceptions.BadEntrySpecificationException;
import m19.exceptions.UnknownDataException;
import m19.rules.*;

/**
 * Class that represents the library as a whole.
 */
public class Library implements Serializable {

	/** Serial number for serialization. */
	private static final long serialVersionUID = 201901101348L;

	/** Current user id */
	private int _userId = 0;

	/** Current work id */
	private int _workId = 0;

	/** Current date */
	private int _date = 0;

	/** Users registered in the library */
	private Map<Integer, User> _users = new TreeMap<Integer, User>();

	/** Works registered in the library */
	private Map<Integer, Work> _works = new TreeMap<Integer, Work>();

	/** Rules to make a request */
	private List<Rule> _rules = new ArrayList<>();

	/**
	 * 
	 * @param filename
	 * @throws BadEntrySpecificationException
	 * @throws IOException
	 */
	public Library() {
		_rules.add(new DuplicateWorkRequestRule());
		_rules.add(new SuspenseUserRule());
		_rules.add(new NoCopiesRule());
		_rules.add(new MaxNumberRequestsRule());
		_rules.add(new ReferenceWorksRule());
		_rules.add(new MaxWorkPriceRule());
	}


	// File management related functions
	
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
  		while ((line = reader.readLine()) != null) {
			String[] fields = line.split(":");
			try {
				registerFromFields(fields);
			} catch (UnknownDataException e) {
				throw new BadEntrySpecificationException(e.getData());
			}
		}
		reader.close();
	}

	/**
	 * Parse the data imported from the file
	 * @param fields
	 * @throws UnknownDataException
	 * @see m19.Library.importFile
	 */
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

	/**
	 * @return current date
	 */
	public int getDate() {
		return _date;
	}

	/**
	 * Advance the date by the given days
	 * @param daysToAdvance
	 */
	public void advanceDate(int daysToAdvance) {
		_date += daysToAdvance;
	}


	// Users functions

	/**
	 * @return current user id
	 */
	public int getCurrentUserId() {
		return _userId;
	}

	/**
	 * Register a new user
	 * @param name New user's name
	 * @param email New user's email
	 * @return New user's id
	 */
	public int registerUser(String name, String email) {
		int id = _userId++;
		User user = new User(id, name, email);
		_users.put(id, user);
		return id;
	}

	/**
	 * Get the user with the given id.
	 * @param id User's id
	 * @return User
	 */
	public User fetchUser(int id) {
		return _users.get(id);
	}

	/**
	 * Get the string of the user with the given id.
	 * @param id User's id
	 * @return User's string
	 */
	public String getUserString(int id) {
		return _users.get(id).toString();
	}

	/**
	 * Get the notifications of the user with the given id.
	 * @param id User's id
	 * @return User's notifications
	 */
	public String getUserNotifications(int id) {
		return _users.get(id).getNotifications();
	}

	/**
	 * Get the state of the user with the given id.
	 * @param id User's id
	 * @return User's state
	 */
	public boolean getUserState(int id) {
		return _users.get(id).getState();
	}

	/**
	 * @return a list with all the users.
	 */
	public List<User> getUsers() {
		return new ArrayList<User>(_users.values());
	}

	/**
	 * Get the fine of the user with the given id.
	 * @param id User's id
	 * @return User's fine
	 */
	public int getUserFine(int id) {
		return _users.get(id).getFine();
	}

	/**
	 * Pay the fine of the user with the given id.
	 * @param id User's id
	 */
	public void userPayFine(int id) {
		User user = _users.get(id);
		user.payFine();
		user.update();
	}


	// Works functions

	/**
	 * @return current work id
	 */
	public int getCurrentWorkId() {
		return _workId;
	}

	/**
	 * Register a new work
	 * @param fields
	 */
	void registerWork(String[] fields) {
		Work work;
		int id = _workId++;
		int price = Integer.parseInt(fields[3]);
		int copies = Integer.parseInt(fields[6]);
		if (fields[0].equals("DVD")) {
			work = new Dvd(id, fields[1], fields[2], price, fields[4], fields[5], copies);
		} else {
			work = new Book(id, fields[1], fields[2], price, fields[4], fields[5], copies);
		}
		_works.put(id, work);
	}

	/**
	 * Get the work with the given id.
	 * @param id Work's id
	 * @return Work
	 */
	public Work fetchWork(int id) {
		return _works.get(id);
	}

	/**
	 * Get the string of the work with the given id
	 * @param id Work's id
	 * @return Work's string
	 */
	public String getWorkString(int id) {
		return _works.get(id).toString();
	}

	/**
	 * Verify if the work with the given id has the given term 
	 * @param id Work's id
	 * @param term Term to search
	 * @return true, if the work has the term; false, otherwise.
	 */
	public boolean workHasTerm(int id, String term) {
		return _works.get(id).hasTerm(term);
	}


	// Requests functions

	/**
	 * Make a request
	 * @param userId User's id
	 * @param workId Work's id
	 * @return 0 in case the request doesn't fail any rule; index of the failed rule otherwise.
	 */
	public int request(int userId, int workId) {
		int index = 0;
		User user = fetchUser(userId);
		Work work = fetchWork(workId);
		for (Rule rule : _rules) {
			index++;
			if (!rule.verify(work, user)) {
				return index;
			}
		}
		//TODO: implement method
		return index;
	}

}
