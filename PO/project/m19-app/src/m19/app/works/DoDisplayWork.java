package m19.app.works;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.3.1. Display work.
 */
public class DoDisplayWork extends Command<LibraryManager> {

	public Input<Integer> id;

	/**
	 * @param receiver
	 */
	public DoDisplayWork(LibraryManager receiver) {
		super(Label.SHOW_WORK, receiver);
		id = _form.addIntegerInput(Message.requestWorkId());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		_display.addLine(_receiver.displayWork(id.value()));
		_display.display();
	}
  
}
