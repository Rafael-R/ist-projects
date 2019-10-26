package m19.users;

import java.util.ArrayList;
import java.util.List;
import m19.works.Observable;

public class User implements Observer {

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