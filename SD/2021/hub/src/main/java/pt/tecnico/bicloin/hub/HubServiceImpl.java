package pt.tecnico.bicloin.hub;

import java.util.*;
import java.util.stream.Collectors;
import static java.lang.Math.*;

import io.grpc.stub.StreamObserver;
import static io.grpc.Status.INVALID_ARGUMENT;
import pt.tecnico.bicloin.hub.grpc.HubServiceGrpc;
import pt.tecnico.bicloin.hub.grpc.Hub.*;
import pt.tecnico.bicloin.frontend.RecFrontend;
import pt.tecnico.rec.grpc.Rec;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;

public class HubServiceImpl extends HubServiceGrpc.HubServiceImplBase {

    RecFrontend frontend;
    int _instance;
    Map<String, User> _users;
    Map<String, Station> _stations;

    public HubServiceImpl(ZKNaming zkNaming, int instance, List<User> users, List<Station> stations, boolean initRec) {
        frontend = new RecFrontend(zkNaming, instance);
        _instance = instance;
        _users = users.stream().collect(Collectors.toMap(User::getUsername, user -> user));
        _stations = stations.stream().collect(Collectors.toMap(Station::getAbbreviation, station -> station));

        if (initRec) {
            initRec();
        }
    }

    private int haversineDistance(double latitude, double longitude, String abbreviation) {
        final int R = 6371000;
        Station station = _stations.get(abbreviation);
        double station_lat = station.getLatitude();
        double station_long = station.getLongitude();
        double latDistance = toRadians(station_lat - latitude);
        double longDistance = toRadians(station_long - longitude);
        double a = sin(latDistance / 2) * sin(latDistance / 2) + cos(toRadians(latitude)) * cos(toRadians(station_lat))
                * sin(longDistance / 2) * sin(longDistance / 2);

        return (int) round(2 * R * asin(sqrt(a)));
    }

    private void initRec() {
        try {
            Rec.InitializeRequest.Builder initialize_req_builder = Rec.InitializeRequest.newBuilder();

            for (Map.Entry<String, User> entry : _users.entrySet()) {
                initialize_req_builder.addUsername(entry.getKey());
            }

            for (Map.Entry<String, Station> entry : _stations.entrySet()) {
                initialize_req_builder.putStations(entry.getKey(), entry.getValue().getBikes());
            }

            Rec.InitializeRequest initialize_req = initialize_req_builder.build();

            frontend.initialize(initialize_req);

        } catch (Exception e) {
            System.out.println("Caught exception " + e + " with message: " + e.getMessage());
        }
    }

    @Override
    public void initRec(InitRecRequest request, StreamObserver<InitRecResponse> responseObserver) {
        initRec();

        InitRecResponse response = InitRecResponse.newBuilder().build();

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void balance(BalanceRequest request, StreamObserver<BalanceResponse> responseObserver) {
        String username = request.getUsername();
        if (!_users.containsKey(username)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid user.").asRuntimeException());
        }

        int balance = 0;

        try {
            Rec.ReadRequest read_req = Rec.ReadRequest.newBuilder()
                    .setRegistry(username)
                    .setType(Rec.Type.USER)
                    .build();

            Rec.ReadResponse read_res = frontend.read(read_req);

            balance = read_res.getUser().getBalance();
        } catch (InterruptedException e) {
            System.out.println("Caught exception " + e + " with message: " + e.getMessage());
        }

        BalanceResponse response = BalanceResponse.newBuilder()
                .setBalance(balance)
                .build();

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void topUp(TopUpRequest request, StreamObserver<TopUpResponse> responseObserver) {
        String username = request.getUsername();
        if (!_users.containsKey(username)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid user.").asRuntimeException());
        }

        int amount = request.getAmount();
        if (amount < 1 || amount > 20) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid amount.").asRuntimeException());
        }

        String phoneNumber = request.getPhoneNumber();
        if (!_users.get(username).getPhoneNumber().equals(phoneNumber)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid user.").asRuntimeException());
        }

        int balance = 0;

        try {
            Rec.ReadRequest read_req = Rec.ReadRequest.newBuilder()
                    .setType(Rec.Type.USER)
                    .setRegistry(username)
                    .build();

            Rec.ReadResponse read_res = frontend.read(read_req);

            balance = read_res.getUser().getBalance();

            balance += amount * 10;

            Rec.UserData user = Rec.UserData.newBuilder()
                    .setBalance(balance)
                    .build();

            Rec.WriteRequest write_req = Rec.WriteRequest.newBuilder()
                    .setType(Rec.Type.USER)
                    .setRegistry(username)
                    .setUser(user)
                    .build();

            frontend.write(write_req);

        } catch (InterruptedException e) {
            System.out.println("Caught exception " + e + " with message: " + e.getMessage());
        }

        TopUpResponse response = TopUpResponse.newBuilder()
                .setBalance(balance)
                .build();

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void infoStation(InfoStationRequest request, StreamObserver<InfoStationResponse> responseObserver) {
        String abbreviation = request.getStation();
        if (!_stations.containsKey(abbreviation)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid station.").asRuntimeException());
        }

        InfoStationResponse response = null;

        try {
            Rec.ReadRequest info_req = Rec.ReadRequest.newBuilder()
                    .setType(Rec.Type.STATION)
                    .setRegistry(abbreviation)
                    .build();

            Rec.ReadResponse info_res = frontend.read(info_req);

            Station station = _stations.get(abbreviation);

            StationData station_info = StationData.newBuilder()
                    .setName(station.getName())
                    .setLat(station.getLatitude())
                    .setLong(station.getLongitude())
                    .setDocks(station.getDocks())
                    .setPrize(station.getPrize())
                    .setBikes(info_res.getStation().getBikes())
                    .setLifts(info_res.getStation().getLifts())
                    .setReturns(info_res.getStation().getReturns())
                    .build();

            response = InfoStationResponse.newBuilder()
                    .setStation(station_info)
                    .build();

        } catch (InterruptedException e) {
            System.out.println("Caught exception " + e + " with message: " + e.getMessage());
        }

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void locateStation(LocateStationRequest request, StreamObserver<LocateStationResponse> responseObserver) {
        double user_lat = request.getLat();
        double user_long = request.getLong();
        int num_stations = request.getNumStations();

        Map<Integer, String> map = new HashMap<>();
        ArrayList<Integer> distance = new ArrayList<>();

        for (String station : _stations.keySet()) {
            int dis = haversineDistance(user_lat, user_long, station);
            map.put(dis, station);
            distance.add(dis);
        }

        Collections.sort(distance);

        LocateStationResponse.Builder response = LocateStationResponse.newBuilder();

        try {
            int stations_map_size = map.size();
            num_stations = min(num_stations, stations_map_size);

            for (int i = 0; i < num_stations; i++) {
                int dis = distance.get(i);
                Station station = _stations.get(map.get(dis));

                Rec.ReadRequest info_req = Rec.ReadRequest.newBuilder()
                        .setType(Rec.Type.STATION)
                        .setRegistry(station.getAbbreviation())
                        .build();

                Rec.ReadResponse info_res = frontend.read(info_req);

                StationData stationData = StationData.newBuilder()
                        .setName(station.getAbbreviation())
                        .setLat(station.getLatitude())
                        .setLong(station.getLongitude())
                        .setDocks(station.getDocks())
                        .setPrize(station.getPrize())
                        .setBikes(info_res.getStation().getBikes())
                        .setDistance(dis)
                        .build();

                response.addStation(stationData);
            }
        } catch (InterruptedException e) {
            System.out.println("Caught exception " + e + " with message: " + e.getMessage());
        }

        LocateStationResponse final_response = response.build();

        responseObserver.onNext(final_response);
        responseObserver.onCompleted();
    }

    @Override
    public void bikeUp(BikeUpRequest request, StreamObserver<BikeUpResponse> responseObserver) {
        String username = request.getUsername();
        if (!_users.containsKey(username)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid user.").asRuntimeException());
        }

        String station = request.getStation();
        if (!_stations.containsKey(station)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid station.").asRuntimeException());
        }

        double latitude = request.getLat();
        double longitude = request.getLong();

        BikeUpResponse response = null;

        if (haversineDistance(latitude, longitude, station) > 200) {
            response = BikeUpResponse.newBuilder()
                    .setResponse(BikeUpResponse.Response.OUT_OF_BOUNDS)
                    .build();
        } else {
            try {
                Rec.ReadRequest user_req = Rec.ReadRequest.newBuilder()
                        .setType(Rec.Type.USER)
                        .setRegistry(username)
                        .build();

                Rec.ReadResponse user_res = frontend.read(user_req);

                Rec.ReadRequest station_req = Rec.ReadRequest.newBuilder()
                        .setType(Rec.Type.STATION)
                        .setRegistry(station)
                        .build();

                Rec.ReadResponse station_res = frontend.read(station_req);

                int balance = user_res.getUser().getBalance();
                boolean hasBike = user_res.getUser().getState();
                int bikes = station_res.getStation().getBikes();
                int lifts = station_res.getStation().getLifts();

                if (balance < 10) {
                    response = BikeUpResponse.newBuilder()
                            .setResponse(BikeUpResponse.Response.INSUFFICIENT_BICLOINS)
                            .build();
                } else if (hasBike) {
                    response = BikeUpResponse.newBuilder()
                            .setResponse(BikeUpResponse.Response.HAS_BIKE)
                            .build();
                } else if (bikes <= 0) {
                    response = BikeUpResponse.newBuilder()
                            .setResponse(BikeUpResponse.Response.NO_BIKES)
                            .build();
                } else {
                    balance -= 10;
                    bikes--;
                    lifts++;

                    Rec.WriteRequest write_req;

                    Rec.Tag userTag = Rec.Tag.newBuilder()
                            .setSeq(user_res.getTag().getSeq() + 1)
                            .setCid(user_res.getTag().getCid())
                            .build();

                    Rec.UserData userData = Rec.UserData.newBuilder()
                            .setBalance(balance)
                            .setState(true)
                            .build();

                    write_req = Rec.WriteRequest.newBuilder()
                            .setType(Rec.Type.USER)
                            .setTag(userTag)
                            .setRegistry(username)
                            .setUser(userData)
                            .build();

                    frontend.write(write_req);

                    Rec.Tag stationTag = Rec.Tag.newBuilder()
                            .setSeq(station_res.getTag().getSeq() + 1)
                            .setCid(station_res.getTag().getCid())
                            .build();

                    Rec.StationData stationData = Rec.StationData.newBuilder()
                            .setBikes(bikes)
                            .setLifts(lifts)
                            .build();

                    write_req = Rec.WriteRequest.newBuilder()
                            .setType(Rec.Type.STATION)
                            .setTag(stationTag)
                            .setRegistry(station)
                            .setStation(stationData)
                            .build();

                    frontend.write(write_req);

                    response = BikeUpResponse.newBuilder()
                            .setResponse(BikeUpResponse.Response.OK)
                            .build();
                }
            } catch (InterruptedException e) {
                System.out.println("Caught exception " + e + " with message: " + e.getMessage());
            }
        }

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void bikeDown(BikeDownRequest request, StreamObserver<BikeDownResponse> responseObserver) {
        String username = request.getUsername();
        if (!_users.containsKey(username)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid user.").asRuntimeException());
        }

        String station = request.getStation();
        if (!_stations.containsKey(station)) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Invalid station.").asRuntimeException());
        }

        double latitude = request.getLat();
        double longitude = request.getLong();

        BikeDownResponse response = null;

        if (haversineDistance(latitude, longitude, station) > 200) {
            response = BikeDownResponse.newBuilder()
                    .setResponse(BikeDownResponse.Response.OUT_OF_BOUNDS)
                    .build();
        } else {
            try {
                Rec.ReadRequest user_req = Rec.ReadRequest.newBuilder()
                        .setType(Rec.Type.USER)
                        .setRegistry(username)
                        .build();

                Rec.ReadResponse user_res = frontend.read(user_req);

                Rec.ReadRequest station_req = Rec.ReadRequest.newBuilder()
                        .setType(Rec.Type.STATION)
                        .setRegistry(station)
                        .build();

                Rec.ReadResponse station_res = frontend.read(station_req);

                int balance = user_res.getUser().getBalance();
                boolean hasBike = user_res.getUser().getState();
                int bikes = station_res.getStation().getBikes();
                int returns = station_res.getStation().getReturns();

                if (!hasBike) {
                    response = BikeDownResponse.newBuilder()
                            .setResponse(BikeDownResponse.Response.NO_BIKE)
                            .build();
                } else if (bikes == _stations.get(station).getDocks()) {
                    response = BikeDownResponse.newBuilder()
                            .setResponse(BikeDownResponse.Response.NO_DOCKS)
                            .build();
                } else {
                    balance += _stations.get(station).getPrize();
                    bikes++;
                    returns++;

                    Rec.WriteRequest write_req;

                    Rec.Tag userTag = Rec.Tag.newBuilder()
                            .setSeq(user_res.getTag().getSeq() + 1)
                            .setCid(user_res.getTag().getCid())
                            .build();

                    Rec.UserData userData = Rec.UserData.newBuilder()
                            .setBalance(balance)
                            .setState(false)
                            .build();

                    write_req = Rec.WriteRequest.newBuilder()
                            .setType(Rec.Type.USER)
                            .setTag(userTag)
                            .setRegistry(username)
                            .setUser(userData)
                            .build();

                    frontend.write(write_req);

                    Rec.Tag stationTag = Rec.Tag.newBuilder()
                            .setSeq(station_res.getTag().getSeq() + 1)
                            .setCid(station_res.getTag().getCid())
                            .build();

                    Rec.StationData stationData = Rec.StationData.newBuilder()
                            .setBikes(bikes)
                            .setReturns(returns)
                            .build();

                    write_req = Rec.WriteRequest.newBuilder()
                            .setType(Rec.Type.STATION)
                            .setTag(stationTag)
                            .setRegistry(station)
                            .setStation(stationData)
                            .build();

                    frontend.write(write_req);

                    response = BikeDownResponse.newBuilder()
                            .setResponse(BikeDownResponse.Response.OK)
                            .build();
                }
            } catch (InterruptedException e) {
                System.out.println("Caught exception " + e + " with message: " + e.getMessage());
            }
        }

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void ping(PingRequest request, StreamObserver<PingResponse> responseObserver) {
        String input = request.getInputText();
        if (input.isBlank()) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Input cannot be empty!")
                    .asRuntimeException());
        }
        String output = "Hello " + input + "!";
        PingResponse response = PingResponse.newBuilder().setOutputText(output).build();

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void sysStatus(StatusRequest request, StreamObserver<StatusResponse> responseObserver) {
        try {
            StatusResponse.Builder responseBuilder = StatusResponse.newBuilder();

            // Add own path
            responseBuilder.addStatus("/grpc/bicloin/hub/" + _instance + " UP");

            List<Rec.PingResponse> ping_res = frontend.ping();

            // Add each rec's response
            for (Rec.PingResponse response : ping_res) {
                responseBuilder.addStatus(response.getResponse());
            }

            StatusResponse response = responseBuilder.build();

            responseObserver.onNext(response);
            responseObserver.onCompleted();
        } catch (InterruptedException e) {
            System.out.println("Caught exception " + e + " with message: " + e.getMessage());
        }
    }
}
