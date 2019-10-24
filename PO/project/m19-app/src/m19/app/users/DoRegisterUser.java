package m19.app.users;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.2.1. Register new user.
 */
public class DoRegisterUser extends Command<LibraryManager> {

	private Input<String> name;
	private Input<String> email;

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
	public final void execute() throws DialogException, UserRegistrationFailedException {
		_form.parse();
		try {
			_receiver-registerUser(name.value(), email.value());
		} catch () {
			throw new UserRegistrationFailedException(name.value(), email.value());
		}
	}

}
