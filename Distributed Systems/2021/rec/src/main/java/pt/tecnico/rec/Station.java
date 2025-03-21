package pt.tecnico.rec;


public class Station {

    private String _name;
    private int _bikes;
    private int _lifts = 0;
    private int _returns = 0;
    private int _tag = 0;
    private int _cid = 0;

    public Station(String name, int bikes) {
        _name = name;
        _bikes = bikes;
    }

    public int getBikes() { return _bikes; }

    public int getLifts() { return _lifts; }

    public int getReturns() { return _returns; }

    public int getTag() {
        return _tag;
    }

    public int getCid() { return _cid; }

    public void setBikes(int bikes) { _bikes = bikes; }

    public void setLifts(int lifts) { _lifts = lifts; }

    public void setReturns(int returns) { _returns = returns; }

    public void setTag(int tag) {
        _tag = tag;
    }

    public void set_cid(int cid) { _cid = cid; }

    @Override
    public String toString() {
        return _name + "|" + _bikes + "|" + _lifts + "|" + _returns;
    }
}
