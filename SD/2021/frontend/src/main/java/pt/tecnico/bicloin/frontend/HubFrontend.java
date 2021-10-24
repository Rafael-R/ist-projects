package pt.tecnico.bicloin.frontend;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.ulisboa.tecnico.sdis.zk.ZKRecord;
import pt.tecnico.bicloin.hub.grpc.HubServiceGrpc;
import pt.tecnico.bicloin.hub.grpc.Hub.*;


public class HubFrontend {

    private final ZKNaming _zkNaming;
    private final String _server_path = "/grpc/bicloin/hub/1";

    public HubFrontend(String host, String port) {
        _zkNaming = new ZKNaming(host, port);
    }

    public HubFrontend(ZKNaming zkNaming) {
        _zkNaming = zkNaming;
    }

    public void initRec(InitRecRequest initRec_req) {
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            InitRecResponse initRec_res = stub.initRec(initRec_req);

            channel.shutdownNow();
        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }
    }

    public BalanceResponse balance(BalanceRequest balance_req) {
        BalanceResponse balance_res = null;
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            balance_res = stub.balance(balance_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return balance_res;
    }

    public TopUpResponse topUp(TopUpRequest topUp_req) {
        TopUpResponse topUp_res = null;
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            topUp_res = stub.topUp(topUp_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return topUp_res;
    }

    public InfoStationResponse infoStation(InfoStationRequest infoStation_req) {
        InfoStationResponse infoStation_res = null;
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            infoStation_res = stub.infoStation(infoStation_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return infoStation_res;
    }

    public LocateStationResponse locateStation(LocateStationRequest locateStation_req) {
        LocateStationResponse locateStation_res = null;
        try {
            ZKRecord rec = _zkNaming.lookup(_server_path);
            String url = rec.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            locateStation_res = stub.locateStation(locateStation_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return locateStation_res;
    }

    public BikeUpResponse bikeUp(BikeUpRequest bikeUp_req) {
        BikeUpResponse bikeUp_res = null;
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            bikeUp_res = stub.bikeUp(bikeUp_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return bikeUp_res;
    }

    public BikeDownResponse bikeDown(BikeDownRequest bikeDown_req) {
        BikeDownResponse bikeDown_res = null;
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            bikeDown_res = stub.bikeDown(bikeDown_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return bikeDown_res;
    }

    public PingResponse ping(PingRequest ping_req) {
        PingResponse ping_res = null;
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            ping_res = stub.ping(ping_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return ping_res;
    }

    public StatusResponse sysStatus(StatusRequest status_req) {
        StatusResponse status_res = null;
        try {
            ZKRecord record = _zkNaming.lookup(_server_path);
            String url = record.getURI();

            final ManagedChannel channel = ManagedChannelBuilder.forTarget(url).usePlaintext().build();

            HubServiceGrpc.HubServiceBlockingStub stub = HubServiceGrpc.newBlockingStub(channel);

            status_res = stub.sysStatus(status_req);

            channel.shutdownNow();

        } catch (ZKNamingException zke) {
            System.out.println("Caught ZooKeeper exception with message: " + zke.getMessage());
        }

        return status_res;
    }
}