package pt.tecnico.distledger.adminclient.grpc;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.Status;
import io.grpc.StatusRuntimeException;
import pt.tecnico.distledger.adminclient.AdminClientMain;
import pt.tecnico.distledger.contract.admin.AdminDistLedger.*;
import pt.tecnico.distledger.contract.admin.AdminServiceGrpc;
import pt.tecnico.distledger.namingserver.grpc.NamingServerService;

import java.util.List;

public class AdminService implements AutoCloseable {

    private static final String SERVICE = "DistLedger";
    private static final String CAUGHT_EXCEPTION = "Caught exception with description: ";
    private final NamingServerService namingServer;
    private ManagedChannel channel;
    private AdminServiceGrpc.AdminServiceBlockingStub stub;
    private String current;

    /**
     * Constructor for admin service to interface with server
     */
    public AdminService() {
        this.namingServer = new NamingServerService();
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
        this.stub = AdminServiceGrpc.newBlockingStub(channel);
        AdminClientMain.debug("Created channel for target: " + target);
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

    /**
     * Activate server with the given code
     *
     * @param qualifier server code
     */
    public void activate(String qualifier) {
        ActivateRequest request = ActivateRequest.newBuilder().build();

        try {
            findServer(qualifier);
            stub.activate(request);
            System.out.println("OK");
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Server " + qualifier + " is currently unavailable.");
        } catch (StatusRuntimeException e) {
            if (e.getStatus().getCode().toStatus().equals(Status.UNAVAILABLE)) {
                System.out.println("Server " + qualifier + " is currently unavailable.");
            } else {
                System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
            }
        }
    }

    /**
     * Deactivate server with the given code
     *
     * @param qualifier server code
     */
    public void deactivate(String qualifier) {
        DeactivateRequest request = DeactivateRequest.newBuilder().build();

        try {
            findServer(qualifier);
            stub.deactivate(request);
            System.out.println("OK");
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Server " + qualifier + " is currently unavailable.");
        } catch (StatusRuntimeException e) {
            if (e.getStatus().getCode().toStatus().equals(Status.UNAVAILABLE)) {
                System.out.println("Server " + qualifier + " is currently unavailable.");
            } else {
                System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
            }
        }
    }

    /**
     *
     *
     * @param qualifier server code
     */
    public void gossip(String qualifier) {
        GossipRequest request = GossipRequest.newBuilder().build();

        // TODO: verify exceptions
        try {
            findServer(qualifier);
            stub.gossip(request);
            System.out.println("OK");
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Server " + qualifier + " is currently unavailable.");
        } catch (StatusRuntimeException e) {
            if (e.getStatus().getCode().toStatus().equals(Status.UNAVAILABLE)) {
                System.out.println("Server " + qualifier + " is currently unavailable.");
            } else {
                System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
            }
        }
    }

    /**
     * Get the ledger content of the server with the given code
     *
     * @param qualifier server code
     */
    public void getLedgerState(String qualifier) {
        getLedgerStateRequest request = getLedgerStateRequest.newBuilder().build();

        try {
            findServer(qualifier);
            getLedgerStateResponse response = stub.getLedgerState(request);
            System.out.println("OK");
            System.out.println(response);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("Server " + qualifier + " is currently unavailable.");
        } catch (StatusRuntimeException e) {
            if (e.getStatus().getCode().toStatus().equals(Status.UNAVAILABLE)) {
                System.out.println("Server " + qualifier + " is currently unavailable.");
            } else {
                System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
            }
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
