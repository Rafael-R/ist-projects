package m19;

import m19.Observer;

public interface Observable {

    public void addObserver(Observer observer);

    public void removeObserver(Observer observer);

    public void notify(String message);

}