package m19.works;

import java.io.Serializable;
import java.util.ArrayList;

import m19.users.Notification;
import m19.users.Observer;

public abstract class Work implements Serializable, Observable {

    enum Category { 
        FICTION, SCITECH, REFERENCE; 
    } 

    private static final long serialVersionUID = 201911092305L;
    
    private int _id;
    private int _copies;
    private int _availableCopies;
    private String _title;
    private int _price;
    private Category _category;
    private ArrayList<Observer> _observers = new ArrayList<Observer>();

    public Work(int id, String title, int price, String category, int copies) {
        _id = id;
        _title = title;
        _price = price;
        switch (category) {
            case "FICTION":
                _category = Category.FICTION;
                break;
            case "SCITECH":
                _category = Category.SCITECH;
                break;
            case "REFERENCE":
                _category = Category.REFERENCE;
                break;
            default:
                break;
        }
        _copies = copies;
        _availableCopies = copies;
    }

    public int getId() {
        return _id;
    }

    public int getPrice() {
        return _price;
    }

    public String getCategory() {
        return _category.toString();
    }

    public void requestCopie() {
        _availableCopies--;
        notifyObservers("REQUISIÇÃO");
    }

    public void returnCopie() {
        _availableCopies++;
        if (_availableCopies == 1) {
            notifyObservers("ENTREGA");
        }
    }

    public int getCopies() {
        return _availableCopies;
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

    @Override
    public void addObserver(Observer observer) {
        _observers.add(observer);
    }

    @Override
    public void removeObserver(Observer observer) {
        _observers.remove(observer);
    }

    @Override
    public void notifyObservers(String message) {
        Notification notification = new Notification(message + ": " + this.toString());
        for (Observer observer : _observers) {
            observer.notify(notification);
        }
    }

    public String categoryString() {
        switch (_category) {
            case FICTION:
                return "Ficção";
            case SCITECH:
                return "Técnica e Científica";
            case REFERENCE:
                return "Referência";
            default:
                return "ERROR";
        }
    }

    public abstract String typeString();

    public abstract String infoString();

    @Override
    public String toString() {
        return _id + " - " + _availableCopies + " de " + _copies + typeString() +
        _title + " - " + _price + " - " + categoryString() + " - " + infoString();
    }
}