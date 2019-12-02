package m19.requests;

public class Request {

    private int _workId;
    private int _returnDay;
    private int _fine = 0;
    private boolean _state = false;

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

    public void setFine(int value) {
        _fine = value;
    }

    public boolean getState() {
        return _state;
    }

    public void changeState() {
        _state = !_state;
    }

    public void update(int day) {
        if (day > _returnDay) {
            _fine = (day - _returnDay) * 5;
        }
    }

    public boolean verifyReturn() {
        if (_state == true && _fine == 0) {
            return true;
        } else {
            return false;
        }
    }
}