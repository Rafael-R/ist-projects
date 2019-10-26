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

    public Work(int id, String title, int price, String category, int copies) {
        _id = id;
        _numberCopies = numberCopies;
        _actualCopies = _numberCopies;
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