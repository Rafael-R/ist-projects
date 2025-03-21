package pt.tecnico.rec;


public class User {

    private final String _name;
    private int _balance = 0;
    private boolean _bike = false;
    private int _tag = 0;
    private int _cid = 0;

    public User(String name) {
        _name = name;
    }

    public int getBalance() { return _balance; }

    public boolean hasBike() { return _bike; }

    public int getTag() {
        return _tag;
    }

    public int getCid() { return _cid; }

    public void setBalance(int balance) { _balance = balance; }

    public void setBike(boolean state) { _bike = state; }

    public void setTag(int tag) {
        _tag = tag;
    }

    public void set_cid(int cid) { _cid = cid; }

    public String toString() {
        return _name + "|" +  _balance + "|" +  _bike + "|" + _cid + "|" + _tag;
    }
}
