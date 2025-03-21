package pt.tecnico.bicloin.frontend;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.ulisboa.tecnico.sdis.zk.ZKRecord;
import pt.tecnico.rec.grpc.RecordServiceGrpc;
import pt.tecnico.rec.grpc.Rec.*;

public class OldRecFrontend {

    private ZKNaming _zkNaming;
    private String _server_path = "/grpc/bicloin/rec/1";

    public OldRecFrontend(String host, String port) {
        _zkNaming = new ZKNaming(host, port);
    }

    public OldRecFrontend(ZKNaming zkNaming) {
        _zkNaming = zkNaming;
    }

    public OldRecFrontend(ZKNaming zkNaming, String server_path) {
        _zkNaming = zkNaming;
        _server_path = server_path;
    }

    public void initialize(InitializeRequest init_req) throws ZKNamingException {
        ZKRecord record = _zkNaming.lookup(_server_path);
        String target = record.getURI();

        final ManagedChannel channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();

        RecordServiceGrpc.RecordServiceBlockingStub stub = RecordServiceGrpc.newBlockingStub(channel);

        InitializeResponse init_res = stub.initialize(init_req);

        channel.shutdownNow();
    }

    public ReadResponse read(ReadRequest read_req) throws ZKNamingException {
        ZKRecord record = _zkNaming.lookup(_server_path);
        String target = record.getURI();

        final ManagedChannel channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();

        RecordServiceGrpc.RecordServiceBlockingStub stub = RecordServiceGrpc.newBlockingStub(channel);

        ReadResponse read_res = stub.read(read_req);

        channel.shutdownNow();

        return read_res;
    }

    public void write(WriteRequest write_req) throws ZKNamingException {
        ZKRecord record = _zkNaming.lookup(_server_path);
        String target = record.getURI();

        final ManagedChannel channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();

        RecordServiceGrpc.RecordServiceBlockingStub stub = RecordServiceGrpc.newBlockingStub(channel);

        WriteResponse write_res = stub.write(write_req);

        channel.shutdownNow();
    }

    public PingResponse ping(PingRequest ping_req) throws ZKNamingException {
        ZKRecord record = _zkNaming.lookup(_server_path);
        String target = record.getURI();

        final ManagedChannel channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();

        RecordServiceGrpc.RecordServiceBlockingStub stub = RecordServiceGrpc.newBlockingStub(channel);

        PingResponse ping_res = stub.ping(ping_req);

        channel.shutdownNow();

        return ping_res;
    }

    public void cleanup(Empty emptyRequest) throws ZKNamingException {
        ZKRecord record = _zkNaming.lookup(_server_path);
        String target = record.getURI();

        final ManagedChannel channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();

        RecordServiceGrpc.RecordServiceBlockingStub stub = RecordServiceGrpc.newBlockingStub(channel);

        Empty empty_res = stub.cleanup(emptyRequest);

        channel.shutdownNow();
    }
}
