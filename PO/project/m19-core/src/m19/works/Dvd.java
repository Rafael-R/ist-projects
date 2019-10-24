package m19.works;

public class Dvd extends Work {
    

    @Override
    public void notifyObservers() {
        for (Observer observer : observers) {
            observer.notify("DISPONIBILIDADE: " + this.toString());
            removeObserver(observer);
        }
    }
    
}