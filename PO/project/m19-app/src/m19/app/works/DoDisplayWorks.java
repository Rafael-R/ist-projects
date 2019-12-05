package m19.app.works;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;

/**
 * 4.3.2. Display all works.
 */
public class DoDisplayWorks extends Command<LibraryManager> {

	/**
	 * @param receiver
	 */
	public DoDisplayWorks(LibraryManager receiver) {
		super(Label.SHOW_WORKS, receiver);
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() {
		_display.popup(_receiver.displayWorks());
	}

}
