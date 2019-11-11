package m19.requests;

public class Request {

    private int _userId;
    private int _workId;
    private int _returnDay;
    private int _fine = 0;
    private boolean _state = false;

    public Request(int userId, int workId, int returnDay) {
        _userId = userId;
        _workId = workId;
        _returnDay = returnDay;
    }

    public int getReturnDay() {
        return _returnDay;
    }

    public void setReturnDay(int returnDay) {
        _returnDay = returnDay;
    }

    public int getFineValue() {
        return _fine;
    }

    public void setFineValue(int value) {
        _fine = value;
    }

    public boolean getState() {
        return _state;
    }

    public void changeState() {
        _state = !_state;
    }

    public void incrementFine(int date) {
        if (_state == false && date > _returnDay) {
            _fine += 5;
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