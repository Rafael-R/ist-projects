package pt.tecnico.distledger.server.grpc;

import io.grpc.stub.StreamObserver;
import pt.tecnico.distledger.contract.admin.AdminServiceGrpc.AdminServiceImplBase;
import pt.tecnico.distledger.contract.admin.AdminDistLedger.*;
import pt.tecnico.distledger.contract.DistLedgerCommonDefinitions.LedgerState;
import pt.tecnico.distledger.contract.DistLedgerCommonDefinitions.OperationType;
import pt.tecnico.distledger.contract.DistLedgerCommonDefinitions;
import pt.tecnico.distledger.server.exception.ServerException;
import pt.tecnico.distledger.server.domain.ServerState;
import pt.tecnico.distledger.server.domain.operation.*;

import java.util.List;

import static io.grpc.Status.*;

public class AdminServiceImpl extends AdminServiceImplBase {

    private final ServerState server;

    public AdminServiceImpl(ServerState server) {
        this.server = server;
    }

    @Override
    public void activate(ActivateRequest request, StreamObserver<ActivateResponse> responseObserver) {
        try {
            server.setActive(true);
            ActivateResponse response = ActivateResponse.newBuilder().build();
            responseObserver.onNext(response);
            responseObserver.onCompleted();
        } catch (ServerException.CurrentState e) {
            responseObserver.onError(FAILED_PRECONDITION
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }

    @Override
    public void deactivate(DeactivateRequest request, StreamObserver<DeactivateResponse> responseObserver) {
        try {
            server.setActive(false);
            DeactivateResponse response = DeactivateResponse.newBuilder().build();
            responseObserver.onNext(response);
            responseObserver.onCompleted();
        } catch (ServerException.CurrentState e) {
            responseObserver.onError(FAILED_PRECONDITION
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }

    @Override
    public void gossip(GossipRequest request, StreamObserver<GossipResponse> responseObserver) {
        try {
            LedgerState ledgerState;

            synchronized (server) {
                ledgerState = buildLedgerState(server.getLedger());
            }

            CrossServerService crossServerService = new CrossServerService();
            crossServerService.propagateState(ledgerState, server.getTS());

            GossipResponse response = GossipResponse.newBuilder().build();
            responseObserver.onNext(response);
            responseObserver.onCompleted();
        } catch (ServerException.UnableToPropagateState e) {
            responseObserver.onError(UNAVAILABLE
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }

    @Override
    public void getLedgerState(getLedgerStateRequest request, StreamObserver<getLedgerStateResponse> responseObserver) {
        LedgerState ledgerState;

        synchronized (server) {
            ledgerState = buildLedgerState(server.getLedger());
        }

        getLedgerStateResponse response = getLedgerStateResponse.newBuilder().setLedgerState(ledgerState).build();
        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    private LedgerState buildLedgerState (List<Operation> ledger) {
        LedgerState.Builder ledgerState = LedgerState.newBuilder();

        DistLedgerCommonDefinitions.Operation.Builder operation = DistLedgerCommonDefinitions.Operation.newBuilder();

        for (Operation op : ledger) {
            operation.setUserId(op.getAccount());
            operation.addAllPrevTS(op.getPrevTS());
            operation.addAllTS(op.getTS());
            if (op instanceof CreateOp) {
                operation.setType(OperationType.OP_CREATE_ACCOUNT);
            } else if (op instanceof TransferOp) {
                operation.setType(OperationType.OP_TRANSFER_TO)
                        .setDestUserId(((TransferOp) op).getDestAccount())
                        .setAmount(((TransferOp) op).getAmount());
            }
            ledgerState.addLedger(operation);
            operation.clear();
        }

        return ledgerState.build();
    }
}
