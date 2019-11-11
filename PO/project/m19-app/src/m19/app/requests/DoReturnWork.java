package m19.app.requests;

import m19.LibraryManager;
import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.NoSuchWorkException;
import m19.app.exceptions.UserIsActiveException;
import m19.app.exceptions.WorkNotBorrowedByUserException;
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
	private Input<String> option;

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
		try {
			_receiver.returnWork(userId.value(), workId.value());
			_display.popup(Message.showFine(userId.value(), _receiver.showUserFine(userId.value())));
			option = _form.addStringInput(Message.requestFinePaymentChoice());
			if (option.value().equals("s")) {
				_receiver.payFine(userId.value());
			}
		} catch (InvalidUserIdException e) {
			throw new NoSuchUserException(e.getId());
		} catch (InvalidWorkIdException e) {
			throw new NoSuchWorkException(e.getId());
		} catch (UserNotSuspendedException e){
			throw new UserIsActiveException(e.getId());
		} catch (WorkNotRequestedByUserException e) {
			throw new WorkNotBorrowedByUserException(e.getWork(), e.getUser());
		}
	}
}
