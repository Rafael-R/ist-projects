package m19.app.users;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;
import m19.app.exceptions.UserRegistrationFailedException;
import m19.exceptions.InvalidDataException;

/**
 * 4.2.1. Register new user.
 */
public class DoRegisterUser extends Command<LibraryManager> {

	private Input<String> name;
	private Input<String> email;
	private int id;

	/**
	 * @param receiver
	 */
	public DoRegisterUser(LibraryManager receiver) {
		super(Label.REGISTER_USER, receiver);
		name = _form.addStringInput(Message.requestUserName());
		email = _form.addStringInput(Message.requestUserEMail());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try {
			id = _receiver.registerUser(name.value(), email.value());
			_display.popup(Message.userRegistrationSuccessful(id));
		} catch (InvalidDataException e) {
			throw new UserRegistrationFailedException(e.getName(), e.getEmail());
		}
	}

}
