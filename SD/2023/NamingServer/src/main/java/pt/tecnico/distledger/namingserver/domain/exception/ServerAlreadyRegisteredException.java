package pt.tecnico.distledger.namingserver.domain.exception;

public class ServerAlreadyRegisteredException extends RuntimeException {
    private final String message;

    public ServerAlreadyRegisteredException(String address) {
        this.message = "Server with address '" + address + "' is already registered.";
    }

    public String getMessage() {
        return this.message;
    }
}
