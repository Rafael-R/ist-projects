package m19.app.requests;

import m19.LibraryManager;
import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.NoSuchWorkException;
import m19.app.exceptions.WorkNotBorrowedByUserException;
import m19.exceptions.FineToPayException;
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
		} catch (InvalidUserIdException e1) {
			throw new NoSuchUserException(e1.getId());
		} catch (InvalidWorkIdException e1) {
			throw new NoSuchWorkException(e1.getId());
		} catch (WorkNotRequestedByUserException e1) {
			throw new WorkNotBorrowedByUserException(e1.getWork(), e1.getUser());
		} catch (FineToPayException e1) {
			_display.popup(Message.showFine(userId.value(), e1.getFine()));
			_form.clear();
			choice = _form.addStringInput(Message.requestFinePaymentChoice());
			_form.parse();
			if (choice.value().equals("s")) {
				_receiver.payFine(userId.value(), workId.value());
			}
			_form.clear();
			userId = _form.addIntegerInput(Message.requestUserId());
			workId = _form.addIntegerInput(Message.requestWorkId());
		}
	}
}
