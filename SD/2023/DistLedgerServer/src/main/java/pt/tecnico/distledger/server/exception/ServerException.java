package pt.tecnico.distledger.server.exception;

public class ServerException {

    public static class Unavailable extends Exception {
        public Unavailable(String message) {
            super(message);
        }
    }

    public static class UserHasFunds extends Exception {
        public UserHasFunds(String message) {
            super(message);
        }
    }

    public static class InsufficientFunds extends Exception {
        public InsufficientFunds(String message) {
            super(message);
        }
    }

    public static class InvalidUser extends Exception {
        public InvalidUser() { super("User is not valid."); }
        public InvalidUser(String message) {
            super(message);
        }
    }

    public static class CurrentState extends Exception {
        public CurrentState(String message) {
            super(message);
        }
    }

    public static class InvalidTransferAmount extends Exception {
        public InvalidTransferAmount(String message) {
            super(message);
        }
    }

    public static class SelfTransferException extends Exception {
        public SelfTransferException(String message) {
            super(message);
        }
    }

    public static class UnableToPropagateState extends Exception {
        public UnableToPropagateState() { super("Unable to propagate state to replica."); }
        public UnableToPropagateState(String message) { super(message); }

    }

}
