package m19.works;

import java.util.ArrayList;
import m19.users.Observer;

public abstract class Work implements Observable {

    private ArrayList<Observer> observers = new ArrayList<Observer>();
    
    protected int _id;
    protected int _numberCopies;
    protected int _actualCopies;
    protected String _title;
    protected int _price;
    protected Category _category;

    public Work(int id, int numberCopies, String title, int price, Category category) {
        _id = id;
        _numberCopies = numberCopies;
        _actualCopies = _numberCopies;
        _title = title;
        _price = price;
        _category = category;
    }

    public void returnWork() {
        _actualCopies++;
        notifyObservers();
    }

    public boolean verifyAvailability() {
        if (_actualCopies == 0) {
            return false;
        } else {
            return true;
        }
    }

    public abstract boolean hasTerm(String term);

    public void addObserver(Observer observer) {
        observers.add(observer);
    }

    public void removeObserver(Observer observer) {
        observers.remove(observer);
    }

    @Override
    public void notifyObservers() {
        for (Observer observer : observers) {
            observer.notify("DISPONIBILIDADE: " + this.toString());
            removeObserver(observer);
        }
    }

}