package m19.users;

import m19.works.Observable;

public interface Observer {

    public void subscribe(Observable observable);

    public void unsubscribeAll();

    public void notify(Notification notification);
}