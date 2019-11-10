package m19.works;

import java.io.Serializable;
import java.util.ArrayList;
import m19.users.Observer;

public class Work implements Serializable, Observable {

    private static final long serialVersionUID = 201911092305L;
    
    protected int _id;
    protected int _copies;
    protected int _availableCopies;
    protected String _title;
    protected int _price;
    protected Category _category;
    private ArrayList<Observer> observers = new ArrayList<Observer>();

    public Work(int id, String title, int price, String category, int copies) {
        _id = id;
        _title = title;
        _price = price;
        switch (category) {
            case "FICTION":
                _category = new FictionWork();
                break;
            case "SCITECH":
                _category = new ScitechWork();
                break;
            case "REFERENCE":
                _category = new ReferenceWork();
                break;
            default:
                break;
        }
        _copies = copies;
        _availableCopies = copies;
    }

    public void requestWork() {
        _availableCopies--;
        notifyObservers("REQUISIÇÃO");
    }

    public void returnWork() {
        _availableCopies++;
        notifyObservers("ENTREGA");
    }

    public boolean verifyAvailability() {
        if (_availableCopies == 0) {
            return false;
        } else {
            return true;
        }
    }

    public boolean hasTerm(String term) {
        return _title.toLowerCase().contains(term.toLowerCase());
    }

    public void addObserver(Observer observer) {
        observers.add(observer);
    }

    public void removeObserver(Observer observer) {
        observers.remove(observer);
    }

    @Override
    public void notifyObservers(String message) {
        for (Observer observer : observers) {
            observer.notify(message + ": " + this.toString());
            removeObserver(observer);
        }
    }

}