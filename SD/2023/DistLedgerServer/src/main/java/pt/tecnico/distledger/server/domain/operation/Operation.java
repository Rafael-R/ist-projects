package pt.tecnico.distledger.server.domain.operation;

import java.util.List;

public class Operation {

    private String account;
    private List<Integer> prevTS;
    private List<Integer> ts;
    private boolean stable;

    public Operation(String fromAccount, List<Integer> prevTS, List<Integer> ts) {
        this.account = fromAccount;
        this.prevTS = prevTS;
        this.ts = ts;
        this.stable = false;
    }

    public String getAccount() {
        return account;
    }

    public void setAccount(String account) {
        this.account = account;
    }

    public List<Integer> getPrevTS() { return prevTS; }

    public void setPrevTS(List<Integer> prevTS) { this.prevTS = prevTS; }

    public List<Integer> getTS() {
        return this.ts;
    }

    public void setTS(List<Integer> ts) {
        this.ts = ts;
    }

    public boolean isStable() {
        return this.stable;
    }

    public void setStable(boolean stable) {
        this.stable = stable;
    }

}
