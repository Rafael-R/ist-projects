package m19.app.requests;

import m19.LibraryManager;
import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.NoSuchWorkException;
import m19.app.exceptions.RuleFailedException;
import m19.exceptions.InvalidUserIdException;
import m19.exceptions.InvalidWorkIdException;
import m19.exceptions.RuleVerificationException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

/**
 * 4.4.1. Request work.
 */
public class DoRequestWork extends Command<LibraryManager> {

	private Input<Integer> userId;
	private Input<Integer> workId;
	private Input<String> choice;

	/**
	 * @param receiver
	 */
	public DoRequestWork(LibraryManager receiver) {
		super(Label.REQUEST_WORK, receiver);
		userId = _form.addIntegerInput(Message.requestUserId());
		workId = _form.addIntegerInput(Message.requestWorkId());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try {
			int returnDay = _receiver.requestWork(userId.value(), workId.value());
			_display.popup(Message.workReturnDay(workId.value(), returnDay));
		} catch (InvalidUserIdException e) {
			throw new NoSuchUserException(e.getId());
		} catch (InvalidWorkIdException e) {
			throw new NoSuchWorkException(e.getId());
		} catch (RuleVerificationException e) {
			if (e.getRuleIndex() == 3) {
				_form.clear();
				choice = _form.addStringInput(Message.requestReturnNotificationPreference());
				_form.parse();
				if (choice.value().equals("s")) {
					try {
						_receiver.observe(e.getUser(), e.getWork());
					} catch (InvalidUserIdException | InvalidWorkIdException ee) {
						ee.printStackTrace();
					}
				}
				_form.clear();
				userId = _form.addIntegerInput(Message.requestUserId());
				workId = _form.addIntegerInput(Message.requestWorkId());
			} else {
				throw new RuleFailedException(e.getUser(), e.getWork(), e.getRuleIndex());
			}
		}
	}

}
