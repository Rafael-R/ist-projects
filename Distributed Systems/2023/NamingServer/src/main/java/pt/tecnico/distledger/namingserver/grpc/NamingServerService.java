package pt.tecnico.distledger.namingserver.grpc;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.StatusRuntimeException;
import pt.tecnico.distledger.contract.namingserver.NamingServer.*;
import pt.tecnico.distledger.contract.namingserver.NamingServiceGrpc;

import java.util.List;
import java.util.ArrayList;

public class NamingServerService implements AutoCloseable {

    private static final String CAUGHT_EXCEPTION = "Caught exception with description: ";

    // Communication channel with server
    private final ManagedChannel channel;
    // Server stub
    private final NamingServiceGrpc.NamingServiceBlockingStub stub;

    public NamingServerService() {
        int port = 5001;
        String host = "localhost";
        this.channel = ManagedChannelBuilder.forAddress(host, port).usePlaintext().build();
        this.stub = NamingServiceGrpc.newBlockingStub(channel);
    }

    public void register(String service, String qualifier, String address) {
        RegisterRequest request = RegisterRequest.newBuilder()
                .setService(service)
                .setQualifier(qualifier)
                .setAddress(address)
                .build();

        try {
            stub.register(request);
        } catch (StatusRuntimeException e) {
            System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
        }
    }

    public void delete(String service, String address) {
        DeleteRequest request = DeleteRequest.newBuilder()
                .setService(service)
                .setAddress(address)
                .build();

        try {
            stub.delete(request);
        } catch (StatusRuntimeException e) {
            System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
        }
    }

    public List<String> lookup(String service, String qualifier) {
        LookupRequest request = LookupRequest.newBuilder()
                .setService(service)
                .setQualifier(qualifier)
                .build();

        List<String> response = new ArrayList<>();

        try {
            response = stub.lookup(request).getAddressesList();
        } catch (StatusRuntimeException e) {
            System.out.println(CAUGHT_EXCEPTION + e.getStatus().getDescription());
        }

        return response;
    }

    @Override
    public final void close() {
        channel.shutdown();
    }
}
