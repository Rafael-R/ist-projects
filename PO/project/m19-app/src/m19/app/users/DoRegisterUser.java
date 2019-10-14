package m19.app.users;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.2.1. Register new user.
 */
public class DoRegisterUser extends Command<LibraryManager> {

  // FIXME define input fields

  /**
   * @param receiver
   */
  public DoRegisterUser(LibraryManager receiver) {
    super(Label.REGISTER_USER, receiver);
    // FIXME initialize input fields
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    // FIXME implement command
  }

}
