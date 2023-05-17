package pt.tecnico.distledger.server.domain;

import pt.tecnico.distledger.server.ServerMain;
import pt.tecnico.distledger.server.domain.operation.*;
import pt.tecnico.distledger.server.exception.ServerException;

import java.util.*;

public class ServerState {

    private static final String UNAVAILABLE = "Server is currently unavailable!";
    private static final String INVALID_USER = "' does not have an account!";

    private final int index;
    private boolean active;
    private List<Operation> ledger;
    private Map<String, Integer> accounts;
    private List<Integer> ts;

    public ServerState() {
        this.index = getServerIndex();
        this.active = true;
        this.ledger = Collections.synchronizedList(new ArrayList<>());
        this.accounts = new HashMap<>();
        this.accounts.put("broker", 1000);
        this.ts = new ArrayList<>(Collections.nCopies(3, 0));
    }

    public int getIndex() {
        return index;
    }

    public synchronized void setActive(boolean state) throws ServerException.CurrentState {
        if (active == state) {
            if (state) {
                throw new ServerException.CurrentState("The service is already activated!");
            } else {
                throw new ServerException.CurrentState("The service is already deactivated!");
            }
        }

        this.active = state;
        ServerMain.debug("Server 'active' set to " + state);
    }

    public synchronized boolean isActive() {
        return this.active;
    }

    public synchronized void setLedger(List<Operation> ledger) {
        this.ledger = ledger;
    }

    public synchronized List<Operation> getLedger() {
        ServerMain.debug("Get ledger state");
        return this.ledger;
    }

    public synchronized void setAccounts(Map<String, Integer> accounts) {
        this.accounts = accounts;
    }

    public synchronized Map<String, Integer> getAccounts() {
        return this.accounts;
    }

    public synchronized List<Integer> getTS() {
        ServerMain.debug("Get timestamp -> " + this.ts);
        return this.ts;
    }

    public synchronized int getTS(int index) {
        ServerMain.debug("Get timestamp -> " + this.ts.get(index));
        return this.ts.get(index);
    }

    public synchronized void setTS(List<Integer> ts) {
        this.ts = ts;
        ServerMain.debug("Set timestamp -> " + ts);
    }

    public synchronized void tickTS() {
        int current = ts.get(index);
        ts.set(index, ++current);
        ServerMain.debug("Tick timestamp -> " + this.ts);
    }

    /**
     * Creates a new user account, if it doesn't exist
     * @param op Create Operation
     * @throws ServerException.Unavailable server is not available
     * @throws ServerException.InvalidUser user already has an account
     */
    public synchronized void createAccount(CreateOp op) throws ServerException.Unavailable, ServerException.InvalidUser {
        if (!active) {
            throw new ServerException.Unavailable(UNAVAILABLE);
        } else if (accounts.containsKey(op.getAccount())) {
            throw new ServerException.InvalidUser("'" + op.getAccount() + "' already has an account!");
        }

        this.accounts.put(op.getAccount(), 0);
        ServerMain.debug("Create account '" + op.getAccount() + "'");
    }

    /**
     * Check the balance of given user
     * @param userId user ID
     * @return balance of user
     * @throws ServerException.Unavailable server is not available
     * @throws ServerException.InvalidUser user does not exist
     */
    public synchronized int balance(String userId) throws ServerException.Unavailable, ServerException.InvalidUser {
        if (!active) {
            throw new ServerException.Unavailable(UNAVAILABLE);
        } else if (!accounts.containsKey(userId)) {
            throw new ServerException.InvalidUser("'" + userId + INVALID_USER);
        }

        ServerMain.debug("Check account balance of '" + userId + "'");
        return this.accounts.get(userId);
    }

    /**
     * Transfer given amount of funds from one user to another
     * @param op Transfer Operation
     * @throws ServerException.Unavailable server is not available
     * @throws ServerException.InvalidUser user does not exist
     * @throws ServerException.InsufficientFunds user doesn't have enough funds
     */
    public synchronized void transferTo(TransferOp op) throws ServerException.Unavailable, ServerException.InvalidUser, ServerException.InsufficientFunds {
        if (!active) {
            throw new ServerException.Unavailable(UNAVAILABLE);
        } else if (!accounts.containsKey(op.getAccount())) {
            throw new ServerException.InvalidUser("'" + op.getAccount() + INVALID_USER);
        } else if (!accounts.containsKey(op.getDestAccount())) {
            throw new ServerException.InvalidUser("'" + op.getDestAccount() + INVALID_USER);
        } else if (accounts.get(op.getAccount()) < op.getAmount()) {
            throw new ServerException.InsufficientFunds("'" + op.getAccount() +
                    "' does not have enough funds to complete the transfer!");
        }

        this.accounts.put(op.getAccount(), balance(op.getAccount()) - op.getAmount());
        this.accounts.put(op.getDestAccount(), balance(op.getDestAccount()) + op.getAmount());
        ServerMain.debug("Transfer from '" + op.getAccount() +
                "' to '" + op.getDestAccount() + "' amount '" + op.getAmount() + "'");
    }

    private int getServerIndex() {
        switch (ServerMain.QUALIFIER) {
            case "A": return 0;
            case "B": return 1;
            case "C": return 2;
            default: return -1;
        }
    }
}
