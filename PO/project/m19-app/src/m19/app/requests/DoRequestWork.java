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

	private int userId;
	private int workId;
	private String option;

	/**
	 * @param receiver
	 */
	public DoRequestWork(LibraryManager receiver) {
		super(Label.REQUEST_WORK, receiver);
		userId = _form.addIntegerInput(Message.requestUserId()).value();
		workId = _form.addIntegerInput(Message.requestWorkId()).value();
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		//TODO: todo
		/*try {
			_receiver.requestWork(userId, workId);
			//TODO: implementar ReturnDay().
		} catch (InvalidUserIdException e) {
			throw new NoSuchUserException(e.getId());
		} catch (InvalidWorkIdException e) {
			throw new NoSuchWorkException(e.getId());
		} catch (RuleVerificationException e) {
			if (e.getRuleIndex() == 3) {
				option = _form.addStringInput(Message.requestReturnNotificationPreference()).value();
				if (option.equals("s")) {
					//TODO add obsserver to observable list
				}
			} else {
				throw new RuleFailedException(e.getUser(), e.getWork(), e.getRuleIndex());
			}
		}*/
	}

}
