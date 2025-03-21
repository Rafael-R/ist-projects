package m19.app.works;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;

/**
 * 4.3.3. Perform search according to miscellaneous criteria.
 */
public class DoPerformSearch extends Command<LibraryManager> {

	public Input<String> term;

	/**
	 * @param m
	 */
	public DoPerformSearch(LibraryManager m) {
		super(Label.PERFORM_SEARCH, m);
		term = _form.addStringInput(Message.requestSearchTerm());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() {
		_form.parse();
		_display.popup(_receiver.performSearch(term.value()));
	}
  
}
