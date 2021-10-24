package pt.tecnico.bicloin.frontend;

import com.google.common.util.concurrent.ListenableFuture;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.StatusRuntimeException;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.ulisboa.tecnico.sdis.zk.ZKRecord;
import pt.tecnico.rec.grpc.RecordServiceGrpc;
import pt.tecnico.rec.grpc.Rec.*;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;

public class RecFrontend {

    private final ZKNaming _zkNaming;
    private final int _cid;
    private int quorum;
    //            URI    PATH
    private Map<String, String> recs_uris_targets = new HashMap<>();
    private final long TIMEOUT = 5000;

    public RecFrontend(String host, String port, int cid) {
        _zkNaming = new ZKNaming(host, port);
        _cid = cid;
        updateRecords();
        quorum = recs_uris_targets.size() / 2 + 1;
    }

    public RecFrontend(ZKNaming zkNaming, int cid) {
        _zkNaming = zkNaming;
        _cid = cid;
        updateRecords();
        quorum = recs_uris_targets.size() / 2 + 1;
    }

    public void initialize(InitializeRequest init_req) throws InterruptedException {
        synchronized (this) {
            updateRecords();
        }

        ResponseCollector<InitializeResponse> collector = new ResponseCollector<>();
        ConcurrentMap<String, ListenableFuture<InitializeResponse>> futures = new ConcurrentHashMap<>();
        ArrayList<ManagedChannel> channels = new ArrayList<>();

        for (Map.Entry<String, String> rec : recs_uris_targets.entrySet()) {
            String instance = getInstanceFromRecPath(rec.getValue());
            System.out.format("Contacting replica %s at %s. Sending initialize request...%n", instance, rec.getKey());
            try {
                ManagedChannel channel = ManagedChannelBuilder.forTarget(rec.getKey()).usePlaintext().build();
                channels.add(channel);
                RecordServiceGrpc.RecordServiceFutureStub stub = RecordServiceGrpc.newFutureStub(channel);
                futures.put(instance, stub.withDeadlineAfter(TIMEOUT, TimeUnit.MILLISECONDS).initialize(init_req));
            } catch (StatusRuntimeException e) {
                System.out.println("[X] Init request timeout");
            }
        }

        for (Map.Entry<String, ListenableFuture<InitializeResponse>> future : futures.entrySet()) {
            try {
                InitializeResponse resp = future.getValue().get();
                collector.addResponse(resp);
                System.out.format("Received initialize response from replica %s.%n", future.getKey());
            } catch (ExecutionException e) {
                InitializeResponse initializeResponse = InitializeResponse.newBuilder().build();
                collector.addResponse(initializeResponse);
                System.out.format("Couldn't get init response from replica %s.%n", future.getKey());
            }
        }

        shutdownChannels(channels);
    }

    public ReadResponse read(ReadRequest read_req) throws InterruptedException{
        synchronized (this) {
            updateRecords();
        }

        ResponseCollector<ReadResponse> collector = new ResponseCollector<>();
        ConcurrentMap<String, ListenableFuture<ReadResponse>> futures = new ConcurrentHashMap<>();
        ArrayList<ManagedChannel> channels = new ArrayList<>();

        for (Map.Entry<String, String> rec : recs_uris_targets.entrySet()) {
            String instance = getInstanceFromRecPath(rec.getValue());
            System.out.format("Contacting replica %s at %s. Sending read request...%n", instance, rec.getKey());
            try {
                ManagedChannel channel = ManagedChannelBuilder.forTarget(rec.getKey()).usePlaintext().build();
                channels.add(channel);
                RecordServiceGrpc.RecordServiceFutureStub stub = RecordServiceGrpc.newFutureStub(channel);
                futures.put(instance, stub.withDeadlineAfter(TIMEOUT, TimeUnit.MILLISECONDS).read(read_req));
            } catch (StatusRuntimeException e) {
                System.out.println("[X] Read request timeout");
            }
        }

        int counter = 0;

        for (Map.Entry<String, ListenableFuture<ReadResponse>> future : futures.entrySet()) {
            try {
                ReadResponse resp = future.getValue().get();
                counter++;
                collector.addResponse(resp);
                System.out.format("Received read response from replica %s.%n", future.getKey());
                if (counter == quorum) {
                    break;
                }
            } catch (ExecutionException e) {
                //Empty read response
                ReadResponse readResponse = ReadResponse.newBuilder().build();
                collector.addResponse(readResponse);
                System.out.format("Couldn't get read response from replica %s.%n", future.getKey());
            }
        }

        shutdownChannels(channels);

        Tag tag = Tag.newBuilder()
                .setCid(_cid)
                .setSeq(-1)
                .build();

        ReadResponse readResponseToReturn = ReadResponse.newBuilder()
                .setTag(tag)
                .build();

        for (ReadResponse readRes : collector.getResponses()) {
            if (readRes.getTag().getSeq() > readResponseToReturn.getTag().getSeq()) {
                readResponseToReturn = readRes;
            }
        }

        System.out.format("Frontend received answer with tag %d.%n", readResponseToReturn.getTag().getSeq());

        return readResponseToReturn;
    }

    public void write(WriteRequest write_req) throws InterruptedException{
        synchronized (this) {
            updateRecords();
        }

        ResponseCollector<WriteResponse> collector = new ResponseCollector<>();
        ConcurrentMap<String, ListenableFuture<WriteResponse>> futures = new ConcurrentHashMap<>();
        ArrayList<ManagedChannel> channels = new ArrayList<>();

        ReadRequest read_req = ReadRequest.newBuilder()
                .setType(write_req.getType())
                .setRegistry(write_req.getRegistry())
                .build();

        ReadResponse read_res = read(read_req);

        for (Map.Entry<String, String> rec : recs_uris_targets.entrySet()) {
            String instance = getInstanceFromRecPath(rec.getValue());
            System.out.format("Contacting replica %s at %s. Sending write request...%n", instance, rec.getKey());
            try {
                ManagedChannel channel = ManagedChannelBuilder.forTarget(rec.getKey()).usePlaintext().build();
                channels.add(channel);
                RecordServiceGrpc.RecordServiceFutureStub stub = RecordServiceGrpc.newFutureStub(channel);

                WriteRequest.Builder write_req_builder = WriteRequest.newBuilder()
                        .setType(write_req.getType())
                        .setRegistry(write_req.getRegistry());

                if (write_req.getType() == Type.USER) {
                    write_req_builder.setUser(write_req.getUser());
                } else if (write_req.getType() == Type.STATION) {
                    write_req_builder.setStation(write_req.getStation());
                }

                Tag tag = Tag.newBuilder()
                        .setCid(read_res.getTag().getCid())
                        .setSeq(read_res.getTag().getSeq() + 1)
                        .build();

                write_req_builder.setTag(tag);
                write_req = write_req_builder.build();

                futures.put(instance, stub.withDeadlineAfter(TIMEOUT, TimeUnit.MILLISECONDS).write(write_req));
            } catch (StatusRuntimeException e) {
                System.out.println("[X] Write request timeout");
            }
        }

        int counter = 0;

        for (Map.Entry<String, ListenableFuture<WriteResponse>> future : futures.entrySet()) {
            try {
                WriteResponse resp = future.getValue().get();
                counter++;
                collector.addResponse(resp);
                System.out.format("Received write response from replica %s.%n", future.getKey());
                if (counter == quorum) {
                    break;
                }
            } catch (ExecutionException e) {
                //Empty write response
                WriteResponse writeResponse = WriteResponse.newBuilder().build();
                collector.addResponse(writeResponse);
                System.out.format("Couldn't get write response from replica %s.%n", future.getKey());
            }
        }

        shutdownChannels(channels);
    }

    public List<PingResponse> ping() throws InterruptedException {
        updateRecords();

        ResponseCollector<PingResponse> collector = new ResponseCollector<>();
        ConcurrentMap<String, ListenableFuture<PingResponse>> futures = new ConcurrentHashMap<>();
        ArrayList<ManagedChannel> channels = new ArrayList<>();

        for (Map.Entry<String, String> rec : recs_uris_targets.entrySet()) {
            String instance = getInstanceFromRecPath(rec.getValue());
            System.out.format("Contacting replica %s at %s. Sending ping request...%n", instance, rec.getKey());
            try {
                ManagedChannel channel = ManagedChannelBuilder.forTarget(rec.getKey()).usePlaintext().build();
                channels.add(channel);
                RecordServiceGrpc.RecordServiceFutureStub stub = RecordServiceGrpc.newFutureStub(channel);
                PingRequest ping_req = PingRequest.newBuilder()
                        .setInstance(instance)
                        .build();
                futures.put(instance, stub.withDeadlineAfter(TIMEOUT, TimeUnit.MILLISECONDS).ping(ping_req));
            } catch (StatusRuntimeException e) {
                System.out.println("[X] Ping request timeout");
            }
        }

        for (Map.Entry<String, ListenableFuture<PingResponse>> future : futures.entrySet()) {
            try {
                PingResponse resp = future.getValue().get();
                collector.addResponse(resp);
                System.out.format("Received reply from replica %s.%n", future.getKey());
            } catch (ExecutionException e) {
                PingResponse pingResponse = PingResponse.newBuilder().setResponse("/grpc/bicloin/rec/" + future.getKey() + " DOWN").build();
                collector.addResponse(pingResponse);
                System.out.format("Couldn't get ping response from replica %s.%n", future.getKey());
            }
        }

        shutdownChannels(channels);

        return collector.getResponses();
    }

    public void shutdownChannels(ArrayList<ManagedChannel> channels) {
        for (ManagedChannel channel : channels) {
            channel.shutdownNow();
        }
    }

    public String getInstanceFromRecPath(String rec) {
        //    /grpc/bicloin/rec/X
        //  0   1      2     3  4
        return rec.split("/")[4];
    }

    public void updateRecords() {
        Collection<ZKRecord> records;
        synchronized (this) {
            recs_uris_targets.clear();
        }
        try {
            String server_dir = "/grpc/bicloin/rec";
            records = _zkNaming.listRecords(server_dir);

            for(ZKRecord record : records) {
                synchronized (this) {
                    recs_uris_targets.put(record.getURI(), record.getPath());
                }
            }
        } catch (ZKNamingException zke) {
            System.out.println("Caught exception with message: " + zke.getMessage());
        }
    }
}
