package m19.users;

import java.util.ArrayList;
import java.util.List;
import m19.works.Observable;

public class User implements Observer {

    private int _id;
    private String name = "";
    private String email = "";
    private boolean state = true;
    private Classification _classification;
    private ArrayList<Notification> _notifications = new ArrayList<Notification>();


    public void addNotification(String message) {
        _notifications.add(new Notification(message));
    }

    @Override
    public void notify(String message) {
        addNotification(message);
    }

}