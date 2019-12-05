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

    public int getFine() {
        if (_paid == true) {
            return 0;
        } else {
            return _fine;
        }
    }

    public boolean isReturned() {
        return _returned;
    }

    public void setReturned() {
        _returned = true;
        if (_fine == 0) {
            _paid = true;
        }
    }

    public boolean isPaid() {
        return _paid;
    }

    public void setPaid() {
        _paid = true;
    }

    public int update(int day) {
        if (day > _returnDay) {
            if (_returned == true && _paid == false) {
                return 1;
            } else if (_returned == false) {
                _fine = (day - _returnDay) * 5;
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }
}