package m19.app.requests;

import m19.LibraryManager;
import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.NoSuchWorkException;
import m19.app.exceptions.WorkNotBorrowedByUserException;
import m19.exceptions.InvalidUserIdException;
import m19.exceptions.InvalidWorkIdException;
import m19.exceptions.WorkNotRequestedByUserException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

/**
 * 4.4.2. Return a work.
 */
public class DoReturnWork extends Command<LibraryManager> {

	private int userId;
	private int workId;
	private String option;

	/**
	 * @param receiver
	 */
	public DoReturnWork(LibraryManager receiver) {
		super(Label.RETURN_WORK, receiver);
		userId = _form.addIntegerInput(Message.requestUserId()).value();
		workId = _form.addIntegerInput(Message.requestWorkId()).value();
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		//TODO: todo
		/*try {
			_receiver.returnWork(userId, workId);
			_display.popup(Message.showFine(userId, _receiver.showUserFine(userId)));
			option = _form.addStringInput(Message.requestFinePaymentChoice()).value();
			if (option.equals("s")) {
				_receiver.payFine(userId);
			}
		} catch (InvalidUserIdException e) {
			throw new NoSuchUserException(e.getId());
		} catch (InvalidWorkIdException e) {
			throw new NoSuchWorkException(e.getId());
		} catch (WorkNotRequestedByUserException e) {
			throw new WorkNotBorrowedByUserException(e.getWork(), e.getUser());
		}*/
	}
}
