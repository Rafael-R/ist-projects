package m19.app.main;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.1.1. Open existing document.
 */
public class DoOpen extends Command<LibraryManager> {

  // FIXME define input fields if needed

  /**
   * @param receiver
   */
  public DoOpen(LibraryManager receiver) {
    super(Label.OPEN, receiver);
    // FIXME initialize input fields if needed
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    /*try {
      // FIXME implement command
    } catch (FailedToOpenFileException fnfe) {
      throw new FileOpenFailedException(fnfe.getName());
    } catch (ClassNotFoundException | IOException e) {
      e.printStackTrace();
    }*/
  }

}
