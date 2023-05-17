package pt.tecnico.distledger.server.grpc;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.StatusRuntimeException;
import pt.tecnico.distledger.contract.distledgerserver.DistLedgerCrossServerServiceGrpc;
import pt.tecnico.distledger.contract.distledgerserver.CrossServerDistLedger.*;
import pt.tecnico.distledger.contract.DistLedgerCommonDefinitions.*;
import pt.tecnico.distledger.namingserver.grpc.NamingServerService;
import pt.tecnico.distledger.server.exception.ServerException;

import java.util.List;
import java.util.ArrayList;

public class CrossServerService implements AutoCloseable {

    private static final String SERVICE = "DistLedger";
    private final NamingServerService namingServer;
    private List<String> targets;
    // Communication channel with server
    private ManagedChannel channel;
    // Server stub
    private DistLedgerCrossServerServiceGrpc.DistLedgerCrossServerServiceBlockingStub stub;

    /**
     * Constructor for user service to interface with server
     */
    public CrossServerService() {
        this.namingServer = new NamingServerService();
        this.targets = new ArrayList<>();
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
        this.stub = DistLedgerCrossServerServiceGrpc.newBlockingStub(this.channel);
    }

    /**
     * Find secondary server
     */
    private void findServer() {
        String target;

        if (targets.isEmpty()) {
            targets = namingServer.lookup(SERVICE, "B");
        }
        target = targets.get(0); // throws IndexOutOfBoundsException

        createChannel(target);
    }

    public void propagateState(LedgerState state, List<Integer> replicaTS) throws ServerException.UnableToPropagateState {
        try { // Only propagating the latest operation for efficiency
            findServer();
            PropagateStateRequest request = PropagateStateRequest.newBuilder()
                    .setState(state)
                    .addAllReplicaTS(replicaTS)
                    .build();
            stub.propagateState(request);
        } catch (StatusRuntimeException | IndexOutOfBoundsException e) {
            throw new ServerException.UnableToPropagateState();
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
