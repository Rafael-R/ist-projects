package m19.app.main;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.1.1. Open existing document.
 */
public class DoOpen extends Command<LibraryManager> {

	private Input<String> filename;

	/**
	 * @param receiver
	 */
	public DoOpen(LibraryManager receiver) {
		super(Label.OPEN, receiver);
		filename = _form.addStringInput(Message.openFile());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		/*try {
			_form.parse();
			_receiver.load(filename.value());
		} catch (FailedToOpenFileException fnfe) {
			throw new FileOpenFailedException(fnfe.getName());
		} catch (ClassNotFoundException | IOException e) {
			e.printStackTrace();
		}*/
	}
}
