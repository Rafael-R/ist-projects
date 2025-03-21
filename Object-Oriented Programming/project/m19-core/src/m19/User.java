package m19;

import java.io.Serializable;
import java.util.List;
import java.util.ArrayList;

public class User implements Serializable, Comparable<User>, Observer {

    private static final long serialVersionUID = 201911092304L;

    private int _id;
    private String _name;
    private String _email;
    private boolean _active = true;
    private Classification _classification = new Normal(this);
    private List<Notification> _notifications = new ArrayList<Notification>();
    private int _fine = 0;
    private List<Request> _requests = new ArrayList<Request>();


    public User(int id, String name, String email) {
        _id = id;
        _name = name;
        _email = email;
    }

    public int getId() {
        return _id;
    }

    public String getName() {
        return _name;
    }

    public boolean isActive() {
        return _active;
    }

    public void setActive() {
        _active = true;
    }

    public void suspend() {
        _active = false;
    }

    public String getClassification() {
        return _classification.toString();
    }

    public void setClassification(Classification classification) {
        _classification = classification;
    }

    public int getReturnDays(Work work) {
        return _classification.maxReturnDays(work);
    }

    public int getMaxRequests() {
        return _classification.maxRequests();
    }

    public String getNotifications() {
        String string = "";
        for (Notification notification : _notifications) {
            string += notification + "\n";
        }
        _notifications.clear();
        return string;
    }
    
    public int getFine() {
        return _fine;
    }

    public boolean hasFine() {
        return _fine > 0;
    }

    public void payFine() {
        for (Request request : _requests) {
            if (request.isReturned() && !request.isPaid()) {
                request.setPaid();
            }
        }
        _active = true;
    }

    public void addRequest(Request request) {
        _requests.add(request);
    }

    public Request hasRequested(int workId) {
        for (Request request : _requests) {
            if (request.getWorkId() == workId && !request.isReturned()) {
                return request;
            }
        }
        return null;
    }

    public int getActiveRequests() {
        int count = 0;
        for (Request request : _requests) {
            if (!request.isReturned()) {
                count++;
            }
        }
        return count;
    }

    public void update(int day) {
        _fine = 0;
        _active = true;
        for (Request request : _requests) {
            if (request.getStatus(day)) {
                _fine += request.getFine();
                _active = false;
            }
        }
        _classification.update(_requests);
    }

    public void update(Notification notification) {
        _notifications.add(notification);
    }

    @Override
    public int compareTo(User other) {
        if (_name == other.getName()) {
            return Integer.compare(_id, other.getId());
        } else {
            return _name.compareTo(other.getName());
        }
    }

    @Override
    public String toString() {
        String string =  _id + " - " + _name + " - " + _email + " - " +
                         _classification + " - ";

        if (_active == true) {
            string += "ACTIVO";
        } else {
            string += "SUSPENSO - EUR " + _fine;
        }
        return string;
    }
}