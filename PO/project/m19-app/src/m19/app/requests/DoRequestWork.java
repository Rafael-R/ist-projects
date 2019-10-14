package m19.app.requests;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.4.1. Request work.
 */
public class DoRequestWork extends Command<LibraryManager> {

  // FIXME define input fields

  /**
   * @param receiver
   */
  public DoRequestWork(LibraryManager receiver) {
    super(Label.REQUEST_WORK, receiver);
    // FIXME initialize input fields
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    // FIXME implement command
  }

}
