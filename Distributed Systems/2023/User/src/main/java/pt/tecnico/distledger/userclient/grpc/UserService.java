package pt.tecnico.distledger.userclient.grpc;

import static io.grpc.Status.*;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.StatusRuntimeException;
import pt.tecnico.distledger.contract.user.UserDistLedger.*;
import pt.tecnico.distledger.contract.user.UserServiceGrpc;
import pt.tecnico.distledger.namingserver.grpc.NamingServerService;
import pt.tecnico.distledger.userclient.UserClientMain;

import java.util.List;
import java.util.ArrayList;
import java.util.Collections;

public class UserService implements AutoCloseable {

    private static final String SERVICE = "DistLedger";
    private static final String CAUGHT_EXCEPTION = "Caught exception with description: ";
    private final NamingServerService namingServer;
    private ManagedChannel channel;
    private UserServiceGrpc.UserServiceBlockingStub stub;
    private String current;
    private List<Integer> prevTS;

    /**
     * Constructor for user service to interface with server
     */
    public UserService() {
        this.namingServer = new NamingServerService();
        this.prevTS = new ArrayList<>(Collections.nCopies(3, 0));
    }

    /**
     * Create a channel
     *
     * @param target server address
     */
    private void createChannel(String target) {
        if (this.channel != null) {
            this.channel.shutdown();
        }
        this.channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();
        this.stub = UserServiceGrpc.newBlockingStub(channel);
        UserClientMain.debug("Created channel for target: " + target);
    }

    /**
     * Find server with given qualifier
     *
     * @param qualifier server code
     */
    private void findServer(String qualifier) {
        if (current != null && current.equals(qualifier)) {
            return;
        }

        List<String> targets = namingServer.lookup(SERVICE, qualifier);
        createChannel(targets.get(0));  // throws IndexOutOfBoundsException
        current = qualifier;
    }

    private void updateTS(List<Integer> newTS) {
        for (int i = 0; i < 3; i++) {
            if (newTS.get(i) > prevTS.get(i)) {
                prevTS.set(i, newTS.get(i));
            }
        }
        UserClientMain.debug("Client TS: " + prevTS);
    }

    /**
     * Create user account using rpc
     *
     * @param qualifier Server code
     * @param userID Account username
     */
    public void createAccount(String qualifier, String userID) {
        // Compose request
        CreateAccountRequest request = CreateAccountRequest.newBuilder()
                .setUserId(userID)
                .addAllPrevTS(prevTS)
                .build();

        // Send request and display response
        try {
            findServer(qualifier);
            CreateAccountResponse response = stub.createAccount(request);
            updateTS(response.getTSList());
            display(null);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Server " + qualifier + " is currently unavailable.");
        } catch (StatusRuntimeException e) {
            if (e.getStatus().getCode().toStatus().equals(UNAVAILABLE)) {
                System.out.println("Server " + qualifier + " is currently unavailable.");
            } else {
                System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
            }
        }
    }


    /**
     * Get user account balance using rpc
     *
     * @param qualifier Server code
     * @param userID Account username
     */
    public void balance(String qualifier, String userID) {
        // Compose request
        BalanceRequest request = BalanceRequest.newBuilder()
                .setUserId(userID)
                .addAllPrevTS(prevTS)
                .build();

        // Send request and display response
        try {
            findServer(qualifier);
            BalanceResponse response = stub.balance(request);
            updateTS(response.getValueTSList());
            display(Integer.toString(response.getValue()));
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Server " + qualifier + " is currently unavailable.");
        } catch (StatusRuntimeException e) {
            if (e.getStatus().getCode().toStatus().equals(UNAVAILABLE)) {
                System.out.println("Server " + qualifier + " is currently unavailable.");
            } else {
                System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
            }
        }
    }

    /**
     * Transfer funds to from/to account
     *
     * @param qualifier Server code
     * @param from   Source account to transfer funds from
     * @param dest   Destination account to transfer funds to
     * @param amount Amount of funds to transfer
     */
    public void transferTo(String qualifier, String from, String dest, Integer amount) {
        // Compose request
        TransferToRequest request = TransferToRequest.newBuilder()
                .setAccountFrom(from)
                .setAccountTo(dest)
                .setAmount(amount)
                .addAllPrevTS(prevTS)
                .build();

        // Send request and display response
        try {
            findServer(qualifier);
            TransferToResponse response = stub.transferTo(request);
            updateTS(response.getTSList());
            display(null);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Server " + qualifier + " is currently unavailable.");
        } catch (StatusRuntimeException e) {
            if (e.getStatus().getCode().toStatus().equals(UNAVAILABLE)) {
                System.out.println("Server " + qualifier + " is currently unavailable.");
            } else {
                System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
            }
        }
    }

    /**
     * Display operation status message to standard output
     *
     * @param message Message to display
     */
    private void display(String message) {
        System.out.println("OK");

        if (message != null) {
            System.out.println(message);
        }
    }

    @Override
    public final void close() {
        if (channel != null) {
            channel.shutdown();
        }
        namingServer.close();
    }
}
