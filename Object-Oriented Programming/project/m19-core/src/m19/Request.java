package m19;

import java.io.Serializable;

public class Request implements Serializable {

    private static final long serialVersionUID = 201912041646L;

    private int _workId;
    private int _returnDay;
    private int _fine = 0;
    private boolean _returned = false;
    private boolean _paid = false;
    private boolean _wasSuspended = false;

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

    public int getFine() {
        return _fine;
    }

    public boolean isReturned() {
        return _returned;
    }

    public void setReturned(int day) {
        _returned = true;
        if (day > _returnDay) {
            _fine = (day - _returnDay) * 5;
            _wasSuspended = true;
        } else {
            _paid = true;
        }
    }

    public boolean isPaid() {
        return _paid;
    }

    public void setPaid() {
        _paid = true;
    }

    public boolean getStatus() {
        return _wasSuspended;
    }
    
    public boolean getStatus(int day) {
        if ((day > _returnDay && _returned == false) || 
            (_returned == true && _paid == false)) {
            return true;
        } else {
            return false;
        }
    }

}