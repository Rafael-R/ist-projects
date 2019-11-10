package m19.users;

import java.io.Serializable;
import java.util.ArrayList;

public class User implements Serializable, Observer {

    private static final long serialVersionUID = 201911092304L;

    private int _id;
    private String _name;
    private String _email;
    private boolean _state = true;
    private Classification _classification = new Normal();
    private int _fine = 0;
    private ArrayList<Notification> _notifications = new ArrayList<Notification>();


    public User(int id, String name, String email) {
        _id = id;
        _name = name;
        _email = email;
    }

    public boolean getState() {
        return _state;
    }

    public void setState(boolean state) {
        _state = state;
    }

    public int getFine() {
        return _fine;
    }

    public void payFine() {
        _state = true;
        _fine = 0;
    }

    public void update() {
        //TODO: update user state
    }

    public String getNotifications() {
        String string = "";
        for (Notification notification : _notifications) {
            string += notification + "\n";
        }
        _notifications.clear();
        return string;
    }

    public void addNotification(String message) {
        _notifications.add(new Notification(message));
    }

    @Override
    public void notify(String message) {
        addNotification(message);
    }

    @Override
    public String toString() {
        String string =  _id + " - " + _name + " - " + _email + " - " +
                         _classification + " - ";

        if (_state == true) {
            string += "ACTIVO";
        } else {
            string += "SUSPENSO" + " - EUR " + _fine;
        }
        return string;
    }
}