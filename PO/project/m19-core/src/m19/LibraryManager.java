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

/**
 * The façade class.
 */
public class LibraryManager {

	private Library _library;
	private String _filename;
	private boolean saved = false;

	public LibraryManager() {
		_library = new Library();
	}


	// File management related functions
	
	public String getFilename() {
		return _filename;
	}

	/**
	 * @throws MissingFileAssociationException
	 * @throws IOException
	 * @throws FileNotFoundException
	 */
	public void save() throws MissingFileAssociationException,
							  IOException,
							  FileNotFoundException {
		try {
			if (!saved) {
				ObjectOutputStream out = new ObjectOutputStream(
									 	 new BufferedOutputStream(
									 	 new FileOutputStream(_filename)));
				out.writeObject(_library);
				out.close();
				saved = true;
			}
		} catch (NullPointerException e) {
			throw new MissingFileAssociationException();
		}
	}

	/**
	 * @param filename
	 * @throws MissingFileAssociationException
	 * @throws IOException
	 */
	public void saveAs(String filename) throws MissingFileAssociationException,
											   IOException {
		_filename = filename;
		save();
	}

	/**
	 * @param filename
	 * @throws FailedToOpenFileException
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	public void load(String filename) throws FailedToOpenFileException,
											 IOException,
											 ClassNotFoundException {
		try {
			ObjectInputStream in = new ObjectInputStream(
								   new BufferedInputStream(
								   new FileInputStream(filename)));
			_library = (Library) in.readObject();
			in.close();
			_filename = filename;
			saved = true;
		} catch (FileNotFoundException e) {
			throw new FailedToOpenFileException(filename);
		}
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

	/**
	 * @param daysToAdvance
	 */
	public void advanceDate(int daysToAdvance) {
		saved = false;
		_library.advanceDate(daysToAdvance);
	}


	// Users functions

	/**
	 * @param name
	 * @param email
	 * @throws InvalidUserDataException
	 */
	public int registerUser(String name, String email) throws InvalidDataException {
		saved = false;
		return _library.registerUser(name, email);
	}

	/**
	 * @param userId
	 * @throws InvalidUserIdException
	 */
	public String showUser(int userId) throws InvalidUserIdException {
		return _library.getUserString(userId);
	}

	public List<User> showUsers() {
		return _library.getUsers();
	}

	/**
	 * @param userId
	 * @throws InvalidUserIdException
	 */
	public String showUserNotifications(int userId) throws InvalidUserIdException {
		saved = false;
		return _library.getUserNotifications(userId);
	}

	/**
	 * @param userId
	 * @throws InvalidUserIdException
	 * @throws UserNotSuspendedException
	 */
	public void payFine(int userId) throws InvalidUserIdException,
										   UserNotSuspendedException {
		saved = false;
		_library.payUserFine(userId);
	}


	// Works functions

	/**
	 * @param workId
	 * @throws InvalidWorkIdException
	 */
	public String displayWork(int workId) throws InvalidWorkIdException {
		return _library.getWorkString(workId);
	}

	public String displayWorks() {
		return _library.getWorksString();
	}

	/**
	 * @param term
	 */
	public String performSearch(String term) {
		return _library.performSearch(term);
	}


	// Requests functions

	/**
	 * @param userId
	 * @param workId
	 * @throws InvalidUserIdException
	 * @throws InvalidWorkIdException
	 */
	public void observe(int userId, int workId) throws InvalidUserIdException, 
													    InvalidWorkIdException {
		saved = false;
		_library.observe(userId, workId);
	}

	/**
	 * @param userId
	 * @param workId
	 * @throws InvalidUserIdException
	 * @throws InvalidWorkIdException
	 * @throws RuleVerificationException
	 */
	public int requestWork(int userId, int workId) throws InvalidUserIdException, 
														  InvalidWorkIdException, 
														  RuleVerificationException {
		saved = false;
		return _library.requestWork(userId, workId);
	}

	/**
	 * @param userId
	 * @param workId
	 * @throws InvalidUserIdException
	 * @throws InvalidWorkIdException
	 * @throws WorkNotRequestedByUserException
	 * @throws FineToPayException
	 */
	public void returnWork(int userId, int workId) throws InvalidUserIdException, 
														  InvalidWorkIdException,
														  WorkNotRequestedByUserException,
														  FineToPayException {
		saved = false;
		_library.returnWork(userId, workId);
	}

}
