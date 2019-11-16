package m19;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.List;

import m19.exceptions.*;
import m19.users.User;

/**
 * The façade class.
 */
public class LibraryManager {

	private Library _library = new Library();
	private String _filename;


	// File management related functions

	/**
	 * @throws MissingFileAssociationException
	 * @throws IOException
	 * @throws FileNotFoundException
	 */
	public void save() throws MissingFileAssociationException, IOException, FileNotFoundException {
		try {
			ObjectOutputStream out = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream(_filename)));
			out.writeObject(_library);
			out.close();
		} catch (NullPointerException e) {
			throw new MissingFileAssociationException();
		}
	}

	/**
	 * @param filename
	 * @throws MissingFileAssociationException
	 * @throws IOException
	 */
	public void saveAs(String filename) throws MissingFileAssociationException, IOException {
		_filename = filename;
		save();
	}

	/**
	 * @param filename
	 * @throws FailedToOpenFileException
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	public void load(String filename) throws FailedToOpenFileException, IOException, ClassNotFoundException {
		try {
			ObjectInputStream in = new ObjectInputStream(new BufferedInputStream(new FileInputStream(filename)));
			_library = (Library) in.readObject();
			in.close();
			_filename = filename;
		} catch (FileNotFoundException e) {
			throw new FailedToOpenFileException(filename);
		}
	}

	public String getFilename() {
		return _filename;
	}

	/**
	 * @param datafile
	 * @throws ImportFileException
	 */
	public void importFile(String datafile) throws ImportFileException {
		try {
			_library.importFile(datafile);
		} catch (IOException | BadEntrySpecificationException e) {
			throw new ImportFileException(e);
		}
	}


	// Date functions

	public int getDate() {
		return _library.getDate();
	}

	public void advanceDate(int daysToAdvance) {
		_library.advanceDate(daysToAdvance);
	}


	// Users functions

	public int registerUser(String name, String email) throws InvalidUserDataException {
		if (name.isEmpty() | email.isEmpty()) {
			throw new InvalidUserDataException(name, email);
		} else {
			return _library.registerUser(name, email);
		}
	}

	public String showUser(int id) throws InvalidUserIdException {
		if (_library.fetchUser(id) == null) {
			throw new InvalidUserIdException(id);
		} else {
			return _library.getUserString(id);
		}
	}

	public String showUserNotifications(int id) throws InvalidUserIdException {
		if (_library.fetchUser(id) == null) {
			throw new InvalidUserIdException(id);
		} else {
			return _library.getUserNotifications(id);
		}
	}

	public int showUserFine(int id) throws InvalidUserIdException {
		if (_library.fetchUser(id) == null) {
			throw new InvalidUserIdException(id);
		} else {
			return _library.getUserFine(id);
		}
	}

	public List<User> showUsers() {
		return _library.getUsers();
	}
	
	public void payFine(int id) throws InvalidUserIdException, UserNotSuspendedException {
		if (_library.fetchUser(id) == null) {
			throw new InvalidUserIdException(id);
		} else if (_library.getUserState(id) == true) {
			throw new UserNotSuspendedException(id);
		} else {
			_library.userPayFine(id);
		}
	}


	// Works functions

	public String displayWork(int id) throws InvalidWorkIdException {
		if (_library.fetchWork(id) == null) {
			throw new InvalidWorkIdException(id);
		} else {
			return _library.getWorkString(id);
		}
	}

	public String displayWorks() {
		String string = "";
		for (int i = 0; i < _library.getLastWorkId(); i++) {
			string += _library.getWorkString(i) + "\n";
		}
		return string;
	}

	public String performSearch(String term) {
		String string = "";
		for (int i = 0; i < _library.getLastWorkId(); i++) {
			if (_library.workHasTerm(i, term)) {
				string += _library.getWorkString(i) + "\n";
			}
		}
		return string;
	}

	public void addObserver(int workId, int userId) {
		//TODO: implement method
	}


	// Requests functions

	public int requestWork(int userId, int workId) throws InvalidUserIdException, InvalidWorkIdException, RuleVerificationException {
		if (_library.fetchUser(userId) == null) {
			throw new InvalidUserIdException(userId);
		} else if (_library.fetchWork(workId) == null) {
			throw new InvalidWorkIdException(workId);
		}
		int index = _library.request(userId, workId);
		if (index != 0) {
			throw new RuleVerificationException(userId, workId, 3);
		}
		return 1;
	}

	public void returnWork(int userId, int workId) throws InvalidUserIdException, InvalidWorkIdException, WorkNotRequestedByUserException {
		if (_library.fetchUser(userId) == null) {
			throw new InvalidUserIdException(userId);
		} else if (_library.fetchWork(workId) == null) {
			throw new InvalidWorkIdException(workId);
		}
		//TODO: Define method
		if (userId == workId) {
			throw new WorkNotRequestedByUserException(workId, userId);
		}
	}

}
