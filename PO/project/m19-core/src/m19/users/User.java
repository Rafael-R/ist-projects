package m19.users;

import java.io.Serializable;
import java.util.List;
import java.util.ArrayList;

import m19.requests.Request;
import m19.works.Observable;
import m19.works.Work;


public class User implements Serializable, Comparable<User>, Observer {

    private static final long serialVersionUID = 201911092304L;

    private int _id;
    private String _name;
    private String _email;
    private boolean _state = true;
    private Classification _classification = new Normal();
    private int _fine = 0;
    private List<Request> _requests = new ArrayList<Request>();
    private ArrayList<Observable> _observables = new ArrayList<Observable>();
    private List<Notification> _notifications = new ArrayList<Notification>();


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

    public String getClassification() {
        return _classification.toString();
    }

    public int getReturnDays(Work work) {
        return _classification.maxReturnDays(work);
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

    public void addRequest(Request request) {
        _requests.add(request);
    }

    public String getNotifications() {
        String string = "";
        for (Notification notification : _notifications) {
            string += notification + "\n";
        }
        _notifications.clear();
        return string;
    }

    @Override
    public int compareTo(User other) {
        if (this._name == other._name) {
            return Integer.compare(this._id, other._id);
        } else {
            return this._name.compareTo(other._name);
        }
    }

    @Override
    public void subscribe(Observable observable) {
        _observables.add(observable);
        observable.addObserver(this);
    }

    @Override
    public void unsubscribeAll() {
        for (Observable observable : _observables) {
            observable.removeObserver(this);
        }
        _observables.clear();
    }

    @Override
    public void notify(Notification notification) {
        _notifications.add(notification);
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