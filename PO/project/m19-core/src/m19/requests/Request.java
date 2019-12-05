package m19.requests;

import java.io.Serializable;

public class Request implements Serializable {

    private static final long serialVersionUID = 201912041646L;

    private int _workId;
    private int _returnDay;
    private int _fine = 0;
    private boolean _returned = false;
    private boolean _paid = false;

    public Request(int workId, int returnDay) {
        _workId = workId;
        _returnDay = returnDay;
    }

    public int getWorkId() {
        return _workId;
    }

    public int getReturnDay() {
        return _returnDay;
    }

    public void setReturnDay(int returnDay) {
        _returnDay = returnDay;
    }

    public int getFine() {
        return _fine;
    }

    public boolean isReturned() {
        return _returned;
    }

    public void changeState() {
        _returned = !_returned;
    }

    public boolean isPaid() {
        return _paid == true;
    }

    public void pay() {
        _paid = true;
    }

    public void update(int day) {
        if (day > _returnDay) {
            _fine = (day - _returnDay) * 5;
        }
    }

    public boolean verifyReturn() {
        if (_returned == true && _fine == 0) {
            return true;
        } else {
            return false;
        }
    }
}