package pt.tecnico.distledger.namingserver.grpc;

import io.grpc.stub.StreamObserver;
import pt.tecnico.distledger.contract.namingserver.NamingServiceGrpc.NamingServiceImplBase;
import pt.tecnico.distledger.contract.namingserver.NamingServer.*;
import pt.tecnico.distledger.namingserver.domain.ServerEntry;
import pt.tecnico.distledger.namingserver.domain.ServiceEntry;
import pt.tecnico.distledger.namingserver.domain.exception.ServerAlreadyRegisteredException;
import pt.tecnico.distledger.namingserver.domain.exception.ServerNotRegisteredException;
import pt.tecnico.distledger.namingserver.exception.NoServiceFoundException;

import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.ConcurrentHashMap;

import static io.grpc.Status.*;

public class NamingServerServiceImpl extends NamingServiceImplBase {

    private final ConcurrentMap<String, ServiceEntry> services;

    public NamingServerServiceImpl() {
        this.services = new ConcurrentHashMap<>();
    }

    @Override
    public void register(RegisterRequest request, StreamObserver<RegisterResponse> responseObserver) {
        try {
            if (!services.containsKey(request.getService())) { // If service does not yet exist, create it
                ServiceEntry service = new ServiceEntry(request.getService());
                services.put(request.getService(), service);
            }

            // Register server with service
            ServiceEntry service = services.get(request.getService());
            service.addServerEntry(new ServerEntry(request.getAddress(), request.getQualifier()));

            RegisterResponse response = RegisterResponse.newBuilder().build();
            responseObserver.onNext(response);
            responseObserver.onCompleted();

        } catch (ServerAlreadyRegisteredException e) {
            responseObserver.onError(ALREADY_EXISTS
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }

    @Override
    public void delete(DeleteRequest request, StreamObserver<DeleteResponse> responseObserver) {
        try {
            if (services.containsKey(request.getService())) { // If service exists, delete it
                ServiceEntry service = services.get(request.getService());
                service.removeServerEntry(request.getAddress());

                DeleteResponse response = DeleteResponse.newBuilder().build();
                responseObserver.onNext(response);
                responseObserver.onCompleted();
            } else {
                throw new NoServiceFoundException(request.getService()); // If service does not exist
            }
        } catch (NoServiceFoundException e) {
            responseObserver.onError(NOT_FOUND
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (ServerNotRegisteredException e) {
            responseObserver.onError(FAILED_PRECONDITION
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }

    @Override
    public void lookup(LookupRequest request, StreamObserver<LookupResponse> responseObserver) {
        try {
            if (services.containsKey(request.getService())) { // If service exists
                if (request.getQualifier().isEmpty()) { // If qualifier is empty
                    LookupResponse.Builder response = LookupResponse.newBuilder();
                    for (ServiceEntry serviceEntry : services.values()) {
                        if (serviceEntry.getService().equals(request.getService())) {
                            for (ServerEntry serverEntry : serviceEntry.getServerEntries()) {
                                response.addAddresses(serverEntry.getAddress());
                            }
                        }
                    }

                    responseObserver.onNext(response.build());
                    responseObserver.onCompleted();
                } else { // If a qualifier is provided
                    LookupResponse.Builder response = LookupResponse.newBuilder();
                    for (ServiceEntry serviceEntry : services.values()) {
                        if (serviceEntry.getService().equals(request.getService())) {
                            for (ServerEntry serverEntry : serviceEntry.getServerEntries()) {
                                if (serverEntry.getQualifier().equals(request.getQualifier())) {
                                    response.addAddresses(serverEntry.getAddress());
                                }
                            }
                        }
                    }

                    responseObserver.onNext(response.build());
                    responseObserver.onCompleted();
                }
            } else {
                throw new NoServiceFoundException(request.getService()); // If service does not exist
            }
        } catch (NoServiceFoundException e) {
            responseObserver.onError(NOT_FOUND
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }
}
