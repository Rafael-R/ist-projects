package m19.app.users;

import m19.LibraryManager;
import m19.exceptions.InvalidUserIdException;
import m19.app.exceptions.NoSuchUserException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.2.3. Show notifications of a specific user.
 */
public class DoShowUserNotifications extends Command<LibraryManager> {

	private Input<Integer> id;

	/**
	 * @param receiver
	 */
	public DoShowUserNotifications(LibraryManager receiver) {
		super(Label.SHOW_USER_NOTIFICATIONS, receiver);
		id = _form.addIntegerInput(Message.requestUserId());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try {
			String notifications = _receiver.showUserNotifications(id.value());
			_display.popup(notifications);
		} catch (InvalidUserIdException e) {
			throw new NoSuchUserException(e.getId());
		}
	}

}
