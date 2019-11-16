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

	/** TODO */
	private int _userId = 0;

	/** TODO */
	private int _workId = 0;

	/** TODO */
	private int _date = 0;

	/** TODO */
	private Map<Integer, User> _users = new TreeMap<Integer, User>();

	/** TODO */
	private Map<Integer, Work> _works = new TreeMap<Integer, Work>();

	/** TODO */
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
	 * 
	 * @param fields
	 * @throws UnknownDataException
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
	 * 
	 * @return
	 */
	public int getDate() {
		return _date;
	}

	/**
	 * 
	 * @param daysToAdvance
	 */
	public void advanceDate(int daysToAdvance) {
		_date += daysToAdvance;
	}


	// Users functions

	/**
	 * 
	 * @return
	 */
	public int getLastUserId() {
		return _userId;
	}

	/**
	 * Registers a new user
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
	 * 
	 * @param id
	 * @return
	 */
	public User fetchUser(int id) {
		return _users.get(id);
	}

	/**
	 * 
	 * @param id
	 * @return
	 */
	public String getUserString(int id) {
		return _users.get(id).toString();
	}

	/**
	 * 
	 * @param id
	 * @return
	 */
	public String getUserNotifications(int id) {
		return _users.get(id).getNotifications();
	}

	/**
	 * 
	 * @param id
	 * @return
	 */
	public boolean getUserState(int id) {
		return _users.get(id).getState();
	}

	/**
	 * 
	 * @return
	 */
	public List<User> getUsers() {
		return new ArrayList<User>(_users.values());
	}

	/**
	 * 
	 * @param id
	 * @return
	 */
	public int getUserFine(int id) {
		return _users.get(id).getFine();
	}

	/**
	 * 
	 * @param id
	 */
	public void userPayFine(int id) {
		User user = _users.get(id);
		user.payFine();
		user.update();
	}


	// Works functions

	/**
	 * 
	 * @return
	 */
	public int getLastWorkId() {
		return _workId;
	}

	/**
	 * 
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
	 * 
	 * @param id
	 * @return
	 */
	public Work fetchWork(int id) {
		return _works.get(id);
	}

	/**
	 * 
	 * @param id
	 * @return
	 */
	public String getWorkString(int id) {
		return _works.get(id).toString();
	}

	/**
	 * 
	 * @param id
	 * @param term
	 * @return
	 */
	public boolean workHasTerm(int id, String term) {
		return _works.get(id).hasTerm(term);
	}


	// Requests functions

	/**
	 * 
	 * @param userId 
	 * @param workId
	 * @return index of the failed rule
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
