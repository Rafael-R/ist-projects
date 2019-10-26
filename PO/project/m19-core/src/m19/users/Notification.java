package m19.users;

public class Notification {
    
    private String _message;

    public Notification(String message) {
        _message = message;
    }

    @Override
    public String toString() {
        return _message;
    }
}