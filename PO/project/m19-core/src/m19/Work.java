package m19;

import java.io.Serializable;
import java.util.ArrayList;

public abstract class Work implements Serializable, Observable {

    private static final long serialVersionUID = 201911092305L;
    
    private int _id;
    private String _title;
    private int _price;
    private Category _category;
    private int _copies;
    private int _availableCopies;
    private ArrayList<Observer> _observers = new ArrayList<Observer>();

    public Work(int id, String title, int price, String category, int copies) {
        _id = id;
        _title = title;
        _price = price;
        _category = Category.valueOf(category);
        _copies = copies;
        _availableCopies = copies;
    }

    public int getId() {
        return _id;
    }

    public int getPrice() {
        return _price;
    }

    public Category getCategory() {
        return _category;
    }

    public void requestCopie() {
        _availableCopies--;
        notify("REQUISIÇÃO");
    }

    public void returnCopie() {
        _availableCopies++;
        notify("ENTREGA");
        _observers.clear();
    }

    public int getCopies() {
        return _copies;
    }

    public boolean isAvailable() {
        return _availableCopies > 0;
    }

    public void addObserver(Observer observer) {
        _observers.add(observer);
    }

    public void removeObserver(Observer observer) {
        _observers.remove(observer);
    }

    public void notify(String type) {
        Notification notification = new Notification(type + ": " + this.toString());
        for (Observer observer : _observers) {
            observer.update(notification);
        }
    }

    public abstract boolean infoHasTerm(String term); 

    public boolean titleHasTerm(String term) {
        return _title.toLowerCase().contains(term.toLowerCase());
    }

    public boolean hasTerm(String term) {
        return infoHasTerm(term) || titleHasTerm(term);
    }

    public abstract String typeString();

    public abstract String infoString();

    @Override
    public String toString() {
        return _id + " - " + _availableCopies + " de " + _copies + typeString() +
        _title + " - " + _price + " - " + _category + infoString();
    }
}