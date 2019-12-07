package m19.app.users;

import java.util.List;
import java.util.Collections;

import pt.tecnico.po.ui.Command;
import m19.LibraryManager;
import m19.User;
import m19.UserComparator;

/**
 * 4.2.4. Show all users.
 */
public class DoShowUsers extends Command<LibraryManager> {

	/**
	 * @param receiver
	 */
	public DoShowUsers(LibraryManager receiver) {
		super(Label.SHOW_USERS, receiver);
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() {
		List<User> list = _receiver.showUsers();
		Collections.sort(list, new UserComparator());
		for (User user : list) {
			_display.addLine(user.toString());
		}
		_display.display();
	}
	
}
