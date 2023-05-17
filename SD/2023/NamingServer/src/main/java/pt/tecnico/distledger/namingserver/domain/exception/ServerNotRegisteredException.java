package pt.tecnico.distledger.namingserver.domain.exception;

public class ServerNotRegisteredException extends RuntimeException {
    private final String message;

    public ServerNotRegisteredException(String address) {
        this.message = "Server with address '" + address + "' is not registered.";
    }

    public String getMessage() {
        return this.message;
    }
}
