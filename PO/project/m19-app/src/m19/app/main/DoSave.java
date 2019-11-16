package m19.app.main;

import java.io.IOException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import m19.LibraryManager;
import m19.exceptions.MissingFileAssociationException;

/**
 * 4.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<LibraryManager> {
	
	private Input<String> filename;

	/**
	 * @param receiver
	 */
	public DoSave(LibraryManager receiver) {
		super(Label.SAVE, receiver);
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() {
		try {
			_receiver.save();
		} catch (MissingFileAssociationException e1) {
			filename = _form.addStringInput(Message.newSaveAs());
			try {
				_form.parse();
				_receiver.saveAs(filename.value());
			} catch (MissingFileAssociationException e2) {
				e2.printStackTrace();
			} catch (IOException e2) {
				e2.printStackTrace();
			}
		} catch (IOException e1) {
            e1.printStackTrace();
        }
	}
}
