package m19;

import java.io.Serializable;
import java.io.IOException;
import java.io.FileReader;
import java.io.BufferedReader;
import java.util.Map;
import java.util.TreeMap;
import java.util.List;
import java.util.ArrayList;
import java.util.regex.Pattern;

import m19.rules.*;
import m19.exceptions.*;

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
	private int _day = 0;

	/** Users registered in the library */
	private Map<Integer, User> _users = new TreeMap<Integer, User>();

	/** Works registered in the library */
	private Map<Integer, Work> _works = new TreeMap<Integer, Work>();

	/** Rules to make a request */
	private List<Rule> _rules = new ArrayList<>();

	/**
	 * Library constructor 
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
		try {
			BufferedReader reader = new BufferedReader(
									new FileReader(filename));
			String line;
			while ((line = reader.readLine()) != null) {
				String[] fields = line.split(":");
				registerFromFields(fields);
			}
			reader.close();
		} catch (InvalidDataException e) {
			throw new BadEntrySpecificationException(e.getData(), e);
		}
	}

	/**
	 * Parse the data imported from the file
	 * @param fields
	 * @throws InvalidDataException
	 * @see m19.Library.importFile
	 */
	void registerFromFields(String[] fields) throws InvalidDataException {
		Pattern patternUser = Pattern.compile("^(USER)");
		Pattern patternWork = Pattern.compile("^(DVD|BOOK)");
		if (patternUser.matcher(fields[0]).matches()) {
			registerUser(fields[1], fields[2]);
		} else if (patternWork.matcher(fields[0]).matches()) {
			registerWork(fields);
		} else {
			throw new InvalidDataException(fields[0]);
		}
	}


	// Date functions

	/**
	 * @return current date
	 */
	public int getDate() {
		return _day;
	}

	/**
	 * Advance the date by the given days
	 * @param daysToAdvance
	 */
	public void advanceDate(int daysToAdvance) {
		if(daysToAdvance > 0) {
			_day += daysToAdvance;
			update();
		}
	}

	public void update() {
		for (Map.Entry<Integer, User> entry : _users.entrySet()) {
			User user = entry.getValue();
			user.update(_day);
		}
	}


	// Users functions

	/**
	 * Register a new user
	 * @param name New user's name
	 * @param email New user's email
	 * @return New user's id
	 */
	public int registerUser(String name, String email) throws InvalidDataException {
		if (name.isEmpty() | email.isEmpty()) {
			throw new InvalidDataException(name, email);
		} else {
			int id = _userId++;
			User user = new User(id, name, email);
			_users.put(id, user);
			return id;
		}
	}

	/**
	 * Get the user with the given id.
	 * @param id User's id
	 * @return User
	 */
	public User fetchUser(int userId) throws InvalidUserIdException {
		User user = _users.get(userId);
		if (user == null) {
			throw new InvalidUserIdException(userId);
		} else {
			return user;
		}
	}

	/**
	 * Get the string of the user with the given id.
	 * @param id User's id
	 * @return User's string
	 */
	public String getUserString(int userId) throws InvalidUserIdException {
		User user = fetchUser(userId);
		return user.toString();
	}

	/**
	 * Get the notifications of the user with the given id.
	 * @param userId User's id
	 * @return User's notifications
	 */
	public String getUserNotifications(int userId) throws InvalidUserIdException {
		User user = fetchUser(userId);
		return user.getNotifications();
	}

	/**
	 * @return a list with all the users.
	 */
	public List<User> getUsers() {
		return new ArrayList<User>(_users.values());
	}

	/**
	 * Pay the fine of the user with the given id.
	 * @param id User's id
	 */
	public void payUserFine(int userId) throws InvalidUserIdException, UserNotSuspendedException {
		User user = fetchUser(userId);
		if (user.isActive()) {
			throw new UserNotSuspendedException(userId);
		} else {
			user.payFine();
			user.update(_day);
		}
	}


	// Works functions

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
	 * @param workId Work's id
	 * @return Work
	 */
	public Work fetchWork(int workId) throws InvalidWorkIdException {
		Work work = _works.get(workId);
		if (work == null) {
			throw new InvalidWorkIdException(workId);
		} else {
			return work;
		}
	}

	/**
	 * Get the string of the work with the given id
	 * @param workId Work's id
	 * @return Work's string
	 */
	public String getWorkString(int workId) throws InvalidWorkIdException {
		Work work = fetchWork(workId);
		return work.toString();
	}

	public String getWorksString() {
		String string = "";
		for (Map.Entry<Integer, Work> entry : _works.entrySet()) {
			Work work = entry.getValue();
			string += work.toString() + "\n";
		}
		return string;
	}

	/**
	 * Verify if any work has the given term 
	 * @param id Work's id
	 * @param term Term to search
	 * @return a string with the works that contain the term 
	 */
	public String performSearch(String term) {
		String string = "";
		for (Map.Entry<Integer, Work> entry : _works.entrySet()) {
			Work work = entry.getValue();
			if (work.hasTerm(term)) {
				string += work + "\n";
			}
		}
		return string;
	}


	// Requests functions

	public void observe(int userId, int workId) throws InvalidUserIdException, 
													   InvalidWorkIdException {
		User user = fetchUser(userId);
		Work work = fetchWork(workId);
		work.addObserver(user);
	}

	public int requestWork(int userId, int workId) throws InvalidUserIdException, 
														  InvalidWorkIdException, 
														  RuleVerificationException {
		User user = fetchUser(userId);
		Work work = fetchWork(workId);
		for (Rule rule : _rules) {
			rule.verify(work, user);
		}
		int returnDay = _day + user.getReturnDays(work);
		Request request = new Request(workId, returnDay);
		work.requestCopie();
		user.addRequest(request);
		return returnDay;
	}

	public void returnWork(int userId, int workId) throws InvalidUserIdException, 
														  InvalidWorkIdException, 
														  WorkNotRequestedByUserException, 
														  FineToPayException {
		Work work = fetchWork(workId);
		User user = fetchUser(userId);
		Request request = user.hasRequested(workId);
		if (request == null) {
			throw new WorkNotRequestedByUserException(workId, userId);
		}
		work.returnCopie();
		request.setReturned(_day);
		user.update(_day);
		if (user.hasFine()) {
			throw new FineToPayException(userId, user.getFine());
		}
	}

}
