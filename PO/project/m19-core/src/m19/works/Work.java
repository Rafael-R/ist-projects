package m19.works;

import java.util.ArrayList;
import m19.users.Observer;

public abstract class Work implements Observable {

    private ArrayList<Observer> observers = new ArrayList<Observer>();
    
    private int _id;
    private int _numberCopies;
    private String _title;
    private int _price;

    public Work(int id, int numberCopies, String title, int price) {
        _id = id;
        _numberCopies = numberCopies;
        _title = title;
        _price = price;
    }

    public void returnWork() {
        _numberCopies++;
        notifyObservers();
    }

    public boolean verifyAvailability() {
        if (_numberCopies == 0) {

        }
    }

    public boolean hasTerm(String term) {
        
    }

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