package m19;

import java.io.Serializable;

public class Notification implements Serializable {

    private static final long serialVersionUID = 201912042212L;
    
    private String _message;

    public Notification(String message) {
        _message = message;
    }

    @Override
    public String toString() {
        return _message;
    }
}