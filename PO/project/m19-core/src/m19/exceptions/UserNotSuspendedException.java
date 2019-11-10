package m19.exceptions;

/** Launched when id doesn't match an user. */
public class UserNotSuspendedException extends Exception {
  
  /** Serial number for serialization. */
  private static final long serialVersionUID = 201901101348L;

  /** Bad user id. */
  private int _id;

  /**
   * @param id
   */
  public UserNotSuspendedException(int id) {
    _id = id;
  }

  public int getId() {
    return _id;
  }

}