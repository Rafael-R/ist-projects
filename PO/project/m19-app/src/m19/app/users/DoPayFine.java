package m19.app.users;

import m19.LibraryManager;
import m19.exceptions.InvalidUserIdException;
import m19.exceptions.UserNotSuspendedException;
import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.UserIsActiveException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

/**
 * 4.2.5. Settle a fine.
 */
public class DoPayFine extends Command<LibraryManager> {

	private Input<Integer> id;

	/**
	 * @param receiver
	 */
	public DoPayFine(LibraryManager receiver) {
		super(Label.PAY_FINE, receiver);
		id = _form.addIntegerInput(Message.requestUserId());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try {
			_receiver.payFine(id.value());
		} catch (InvalidUserIdException e) {
			throw new NoSuchUserException(e.getId());
		} catch (UserNotSuspendedException e) {
			throw new UserIsActiveException(e.getId());
		}
	}

}
