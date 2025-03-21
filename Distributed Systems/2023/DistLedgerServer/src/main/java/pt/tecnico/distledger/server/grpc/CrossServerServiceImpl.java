package pt.tecnico.distledger.server.grpc;

import io.grpc.stub.StreamObserver;
import pt.tecnico.distledger.contract.DistLedgerCommonDefinitions;
import pt.tecnico.distledger.contract.DistLedgerCommonDefinitions.LedgerState;
import pt.tecnico.distledger.contract.DistLedgerCommonDefinitions.OperationType;
import pt.tecnico.distledger.contract.distledgerserver.CrossServerDistLedger.PropagateStateRequest;
import pt.tecnico.distledger.contract.distledgerserver.CrossServerDistLedger.PropagateStateResponse;
import pt.tecnico.distledger.contract.distledgerserver.DistLedgerCrossServerServiceGrpc.DistLedgerCrossServerServiceImplBase;
import pt.tecnico.distledger.server.domain.ServerState;
import pt.tecnico.distledger.server.domain.operation.*;

import java.util.List;

public class CrossServerServiceImpl extends DistLedgerCrossServerServiceImplBase {

    private final ServerState server;

    public CrossServerServiceImpl(ServerState state) {
        this.server = state;
    }

    @Override
    public void propagateState(PropagateStateRequest request, StreamObserver<PropagateStateResponse> responseObserver) {

        List<Operation> currentState = server.getLedger();
        LedgerState ledgerState = request.getState();

        List<Integer> prev;
        List<Integer> value;
        int current = 0;

        for (DistLedgerCommonDefinitions.Operation operation : ledgerState.getLedgerList()) {

            value = operation.getPrevTSList();

            for (int i = current; i < currentState.size(); i++) {
                prev = currentState.get(i).getPrevTS();

                if (compareTimeStamps(value, prev) == 1) {

                    Operation op = null;

                    // Do action depending on operation type
                    if (operation.getType() == OperationType.OP_TRANSFER_TO) {
                        op = new TransferOp(
                                operation.getUserId(),
                                operation.getDestUserId(),
                                operation.getAmount(),
                                operation.getPrevTSList(),
                                operation.getTSList()
                        );
                    } else if (operation.getType() == OperationType.OP_CREATE_ACCOUNT) {
                        op = new CreateOp(
                                operation.getUserId(),
                                operation.getPrevTSList(),
                                operation.getTSList());
                    }

                    currentState.add(i, op);
                    current = i;
                }
            }
        }

        // Signal all waiting threads to wake up
        UserServiceImpl.outOfDate.signalAll();

        PropagateStateResponse response = PropagateStateResponse.newBuilder().build();
        responseObserver.onNext(response);
        responseObserver.onCompleted();
    }

    /**
     * Compares two timestamps
     * @param TS_1 Timestamp 1
     * @param TS_2 Timestamp 2
     * @return -1 if TS_1 < TS_2
     * @return 0 if TS_1 is concurrent with TS_2
     * @return 1 if TS_1 > TS_2
     */
    private int compareTimeStamps(List<Integer> TS_1, List<Integer> TS_2) {
        int bigger = 0;
        int smaller = 0;

        for (int i = 0; i < TS_1.size(); i++) {
            if (TS_1.get(i) < TS_2.get(i)) {
                smaller++;
            } else if (TS_1.get(i) > TS_2.get(i)) {
                bigger++;
            }
        }

        if (bigger > 0 && smaller == 0) {
            return 1;
        } else if (smaller > 0 && bigger == 0) {
            return -1;
        }
        return 0;
    }
}
