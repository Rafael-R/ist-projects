package m19.app.requests;

import m19.LibraryManager;
import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.NoSuchWorkException;
import m19.app.exceptions.WorkNotBorrowedByUserException;
import m19.exceptions.FineToPayException;
import m19.exceptions.InvalidUserIdException;
import m19.exceptions.InvalidWorkIdException;
import m19.exceptions.UserNotSuspendedException;
import m19.exceptions.WorkNotRequestedByUserException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

/**
 * 4.4.2. Return a work.
 */
public class DoReturnWork extends Command<LibraryManager> {

	private Input<Integer> userId;
	private Input<Integer> workId;
	private Input<String> choice;

	/**
	 * @param receiver
	 */
	public DoReturnWork(LibraryManager receiver) {
		super(Label.RETURN_WORK, receiver);
		userId = _form.addIntegerInput(Message.requestUserId());
		workId = _form.addIntegerInput(Message.requestWorkId());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try {
			_receiver.returnWork(userId.value(), workId.value());
		} catch (InvalidUserIdException e) {
			throw new NoSuchUserException(e.getId());
		} catch (InvalidWorkIdException e) {
			throw new NoSuchWorkException(e.getId());
		} catch (WorkNotRequestedByUserException e) {
			throw new WorkNotBorrowedByUserException(e.getWork(), e.getUser());
		} catch (FineToPayException e) {
			_display.popup(Message.showFine(e.getUserId(), e.getFine()));
			_form.clear();
			choice = _form.addStringInput(Message.requestFinePaymentChoice());
			_form.parse();
			if (choice.value().equals("s")) {
				try {
					_receiver.payFine(e.getUserId());
				} catch (InvalidUserIdException | UserNotSuspendedException ee) {
					ee.printStackTrace();
				}
				
			}
			_form.clear();
			userId = _form.addIntegerInput(Message.requestUserId());
			workId = _form.addIntegerInput(Message.requestWorkId());
		}
	}
}
