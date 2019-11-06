package m19;

import java.io.IOException;

import m19.exceptions.BadEntrySpecificationException;
import m19.exceptions.FailedToOpenFileException;
import m19.exceptions.ImportFileException;
import m19.exceptions.InvalidNameOrEmailException;
import m19.exceptions.InvalidUserIdException;
import m19.exceptions.InvalidWorkIdException;
import m19.exceptions.MissingFileAssociationException;
import m19.exceptions.UserNotSuspendedException;

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
	public void save() throws MissingFileAssociationException, IOException {
		// FIXME implement method
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
		// FIXME implement method
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

	public String showUsers() {
		String string = "";
		for (int i = 0; i < _library.getLastUserId(); i++) {
			string += _library.getUserString(i) + "\n";
		}
		return string;
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


}
