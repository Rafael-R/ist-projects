package pt.tecnico.distledger.namingserver.exception;

public class NoServiceFoundException extends RuntimeException {
    private final String message;

    public NoServiceFoundException(String service) {
        this.message = "The service '" + service + "' was not found.";
    }

    public String getMessage() {
        return this.message;
    }
}
