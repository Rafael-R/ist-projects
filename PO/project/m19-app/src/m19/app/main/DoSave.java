package m19.app.main;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;

/**
 * 4.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<LibraryManager> {
  
  // FIXME define input fields

  /**
   * @param receiver
   */
  public DoSave(LibraryManager receiver) {
    super(Label.SAVE, receiver);
    // FIXME initialize input fields
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    // FIXME implement command
  }
}
