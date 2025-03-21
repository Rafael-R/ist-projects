package pt.tecnico.rec;

import java.util.Arrays;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import io.grpc.stub.StreamObserver;
import static io.grpc.Status.INVALID_ARGUMENT;

import org.apache.zookeeper.data.Stat;
import pt.tecnico.rec.grpc.RecordServiceGrpc;
import pt.tecnico.rec.grpc.Rec.*;


public class RecServiceImpl extends RecordServiceGrpc.RecordServiceImplBase {

    private ConcurrentMap<String, User> _users = new ConcurrentHashMap<>();
    private ConcurrentMap<String, Station> _stations = new ConcurrentHashMap<>();

    @Override
    public void initialize(InitializeRequest request, StreamObserver<InitializeResponse> responseObserver) {
        _users = new ConcurrentHashMap<>();
        _stations = new ConcurrentHashMap<>();

        String[] usernames = request.getUsernameList().toArray(new String[0]);

        synchronized (this) {
            for (String username : usernames) {
                _users.put(username, new User(username));
            }
        }

        Map<String, Integer> stations = request.getStationsMap();

        synchronized (this) {
            for (Map.Entry<String, Integer> entry : stations.entrySet()) {
                _stations.put(entry.getKey(), new Station(entry.getKey(), entry.getValue()));
            }
        }

        InitializeResponse response = InitializeResponse.newBuilder().build();

        System.out.println("Record initialized");

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void read(ReadRequest request, StreamObserver<ReadResponse> responseObserver) {
        String registry = request.getRegistry();
        if (registry == null || registry.isBlank()) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Registry cannot be empty!")
                    .asRuntimeException());
        }

        Type type = request.getType();

        ReadResponse.Builder response = ReadResponse.newBuilder();

        if (type == Type.USER) {
            System.out.println("Received read request for user: " + registry);
            synchronized (this) {
                UserData.Builder user = UserData.newBuilder();
                if (!_users.containsKey(registry)) {
                    _users.put(registry, new User(registry));
                    user.setBalance(0)
                            .setState(false)
                            .build();
                } else {
                    user.setBalance(_users.get(registry).getBalance())
                            .setState(_users.get(registry).hasBike())
                            .build();

                    Tag tag = Tag.newBuilder()
                            .setSeq(_users.get(registry).getTag())
                            .setCid(_users.get(registry).getCid())
                            .build();

                    response.setTag(tag);
                }
                response.setUser(user);
            }
        } else if (type == Type.STATION) {
            System.out.println("Received read request for station: " + registry);
            synchronized (this) {
                StationData.Builder station = StationData.newBuilder();
                if (!_stations.containsKey(registry)) {
                    _stations.put(registry, new Station(registry, 0));
                    station.setBikes(0).
                            setLifts(0).
                            setReturns(0).
                            build();
                } else {
                    station.setBikes(_stations.get(registry).getBikes())
                            .setLifts(_stations.get(registry).getLifts())
                            .setReturns(_stations.get(registry).getReturns())
                            .build();

                }
                response.setStation(station);

                Tag tag = Tag.newBuilder()
                        .setSeq(_stations.get(registry).getTag())
                        .setCid(_stations.get(registry).getCid())
                        .build();

                response.setTag(tag);
            }
        }

        responseObserver.onNext(response.build());
        responseObserver.onCompleted();
    }

    @Override
    public void write(WriteRequest request, StreamObserver<WriteResponse> responseObserver) {
        String registry = request.getRegistry();
        if (registry == null || registry.isBlank()) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Registry cannot be empty!").
                    asRuntimeException());
        }

        Type type = request.getType();
        Tag tag = request.getTag();
        int t = tag.getSeq();

        if (type == Type.USER) {
            System.out.println("Received write request for user: " + registry);
            synchronized (this) {
                User user = _users.get(registry);
                if ( t > user.getTag()) {
                    user.setBalance(request.getUser().getBalance());
                    if (request.getUser().hasState()) {
                        user.setBike(request.getUser().getState());
                    }
                    user.setTag(t);
                    System.out.println("Data written");
                }
            }
        } else if (type == Type.STATION) {
            System.out.println("Received write request for station: " + registry);
            synchronized (this) {
                Station station = _stations.get(registry);
                if (t > station.getTag()) {
                    station.setBikes(request.getStation().getBikes());
                    if (request.getStation().hasLifts()) {
                        station.setLifts(request.getStation().getLifts());
                    }
                    if (request.getStation().hasReturns()) {
                        station.setReturns(request.getStation().getReturns());
                    }
                    station.setTag(t);
                    System.out.println("Data written");
                }
            }
        }

        responseObserver.onNext(WriteResponse.newBuilder().build());
        responseObserver.onCompleted();
    }

    @Override
    public void ping(PingRequest request, StreamObserver<PingResponse> responseObserver) {
        String instance = request.getInstance();
        if (instance  == null ||instance.isBlank()) {
            responseObserver.onError(INVALID_ARGUMENT.withDescription("Instance cannot be empty!")
                            .asRuntimeException());
        }

        System.out.println("Received ping request");

        PingResponse response = PingResponse.newBuilder().
                setResponse("/grpc/bicloin/rec/" + instance + " UP")
                .build();

        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    @Override
    public void cleanup(Empty empty_req, StreamObserver<Empty> responseObserver){
        synchronized (this) {
            _users.clear();
            _stations.clear();
        }
        responseObserver.onNext(Empty.newBuilder().build());
        responseObserver.onCompleted();
    }
}
