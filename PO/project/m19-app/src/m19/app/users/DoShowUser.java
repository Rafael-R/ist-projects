package m19.app.users;

import m19.LibraryManager;
import m19.app.exceptions.NoSuchUserException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.2.2. Show specific user.
 */
public class DoShowUser extends Command<LibraryManager> {

	private Input<Integer> id;

	/**
	 * @param receiver
	 */
	public DoShowUser(LibraryManager receiver) {
		super(Label.SHOW_USER, receiver);
		id = _form.addIntegerInput(Message.requestUserId());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException, NoSuchUserException {
		_form.parse();
		try {
			_display.addLine(_receiver.showUser(id.value()));
			_display.display();
		} catch (NoSuchUserException e) {
			throw new NoSuchUserException(id.value());
		}
	}

}
