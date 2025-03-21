package pt.tecnico.distledger.server.grpc;

import io.grpc.stub.StreamObserver;
import pt.tecnico.distledger.contract.user.UserServiceGrpc.UserServiceImplBase;
import pt.tecnico.distledger.contract.user.UserDistLedger.*;
import pt.tecnico.distledger.server.ServerMain;
import pt.tecnico.distledger.server.domain.operation.Operation;
import pt.tecnico.distledger.server.exception.ServerException;
import pt.tecnico.distledger.server.domain.ServerState;
import pt.tecnico.distledger.server.domain.operation.*;

import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

import static io.grpc.Status.*;

public class UserServiceImpl extends UserServiceImplBase {

    private final ServerState server;
    private static final ReentrantLock lock = new ReentrantLock();
    public static final Condition outOfDate = lock.newCondition();
    private final int index;

    public UserServiceImpl(ServerState server) {
        this.server = server;
        this.index = server.getIndex();
    }

    @Override
    public void createAccount(CreateAccountRequest request,
                              StreamObserver<CreateAccountResponse> responseObserver) {
        try {
            if (request.getUserId().isEmpty()) { // Check user id is a valid string
                throw new ServerException.InvalidUser();
            }

            List<Operation> ledger = server.getLedger();

            // Checks if it is a repeated operation and does nothing in that case
            for (int i = ledger.size() - 1; i >= request.getPrevTS(index); i--) {
                CreateOp op = (CreateOp) ledger.get(i);
                if (op.getAccount().equals(request.getUserId()) &&
                    op.getTS().equals(request.getPrevTSList())) {
                    return;
                }
            }

            // Increment server timestamp
            server.tickTS();

            // Update server timestamp on vector clock
            List<Integer> prevTS = new ArrayList<>(request.getPrevTSList());
            prevTS.set(index, server.getTS(index));

            // Create new operation
            CreateOp op = new CreateOp(
                    request.getUserId(),
                    request.getPrevTSList(),
                    server.getTS());

            // Add new operation to ledger
            ledger.add(op);

            // Send response
            CreateAccountResponse response = CreateAccountResponse.newBuilder()
                    .addAllTS(prevTS)
                    .build();
            responseObserver.onNext(response);

            // Check and apply operation
            // Check if replica is out of date and wait in that case
            lock.lock();
            try {
                while (compareTimeStamps(request.getPrevTSList(), server.getTS()) == 1) {
                    outOfDate.await();
                }
            } catch (InterruptedException e) {
                throw new ServerException.Unavailable("Error occurred while processing operation.");
            } finally {
                lock.unlock();
            }

            // Apply operation
            op.setStable(true);
            server.createAccount(op);

            responseObserver.onCompleted();
        } catch (ServerException.Unavailable e) {
            responseObserver.onError(UNAVAILABLE
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (ServerException.InvalidUser e) {
            responseObserver.onError(ALREADY_EXISTS
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (UnsupportedOperationException e) {
            responseObserver.onError(UNIMPLEMENTED
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }

    @Override
    public void balance(BalanceRequest request,
                        StreamObserver<BalanceResponse> responseObserver) {
        try {
            if (request.getUserId().isEmpty()) { // Check user id is a valid string
                throw new ServerException.InvalidUser();
            }

            List<Operation> ledger = server.getLedger();

            // Check if replica is out of date and wait in that case
            lock.lock();
            try {
                while (compareTimeStamps(request.getPrevTSList(), server.getTS()) == 1) {
                    outOfDate.await();
                }
            } catch (InterruptedException e) {
                throw new ServerException.Unavailable("Error occurred while processing operation.");
            } finally {
                lock.unlock();
            }

            // Update server timestamp on vector and send response
            List<Integer> prevTS = new ArrayList<>(request.getPrevTSList());
            prevTS.set(index, server.getTS(index));
            BalanceResponse response = BalanceResponse.newBuilder()
                    .setValue(server.balance(request.getUserId()))
                    .addAllValueTS(prevTS)
                    .build();
            responseObserver.onNext(response);
            responseObserver.onCompleted();
        } catch (ServerException.Unavailable e) {
            responseObserver.onError(UNAVAILABLE
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (ServerException.InvalidUser e) {
            responseObserver.onError(NOT_FOUND
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (UnsupportedOperationException e) {
            responseObserver.onError(UNIMPLEMENTED
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
    }

    @Override
    public void transferTo(TransferToRequest request,
                           StreamObserver<TransferToResponse> responseObserver) {
        try {
            if (request.getAccountFrom().equals(request.getAccountTo())) {
                throw new ServerException.SelfTransferException("Not possible to transfer funds to the source account.");
            } else if (request.getAmount() <= 0) {
                throw new ServerException.InvalidTransferAmount("The amount to be transferred is not valid.");
            } else if (request.getAccountFrom().isEmpty() || request.getAccountTo().isEmpty()) {
                // Check source account id and destination account id are valid strings
                throw new ServerException.InvalidUser();
            }

            List<Operation> ledger = server.getLedger();

            // Checks if it is a repeated operation and does nothing in that case
            for (int i = ledger.size() - 1; i >= request.getPrevTS(index); i--) {
                TransferOp op = (TransferOp) ledger.get(i);
                if (op.getAccount().equals(request.getAccountFrom()) &&
                        op.getDestAccount().equals(request.getAccountTo()) &&
                        op.getAmount() == request.getAmount() &&
                        op.getTS().equals(request.getPrevTSList())) {
                    return;
                }
            }

            // Increment server timestamp
            server.tickTS();

            // Update server timestamp on vector
            List<Integer> prevTS = new ArrayList<>(request.getPrevTSList());
            prevTS.set(index, server.getTS(index));

            // Create new operation
            TransferOp op = new TransferOp(
                    request.getAccountFrom(),
                    request.getAccountTo(),
                    request.getAmount(),
                    request.getPrevTSList(),
                    server.getTS());

            // Add new operation to ledger
            ledger.add(op);

            // Send response
            TransferToResponse response = TransferToResponse.newBuilder()
                    .addAllTS(prevTS)
                    .build();
            responseObserver.onNext(response);

            // Check and apply operation
            // Check if replica is out of date and wait in that case
            lock.lock();
            try {
                while (compareTimeStamps(request.getPrevTSList(), server.getTS()) == 1) {
                    outOfDate.await();
                }
            } catch (InterruptedException e) {
                throw new ServerException.Unavailable("Error occurred while processing operation.");
            } finally {
                lock.unlock();
            }

            // Apply operation
            op.setStable(true);
            server.transferTo(op);

            responseObserver.onCompleted();
        } catch (ServerException.Unavailable e) {
            responseObserver.onError(UNAVAILABLE
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (ServerException.InvalidUser e) {
            responseObserver.onError(NOT_FOUND
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (ServerException.InsufficientFunds e) {
            responseObserver.onError(FAILED_PRECONDITION
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (ServerException.InvalidTransferAmount | ServerException.SelfTransferException e) {
            responseObserver.onError(INVALID_ARGUMENT
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        } catch (UnsupportedOperationException e) {
            responseObserver.onError(UNIMPLEMENTED
                    .withDescription(e.getMessage())
                    .asRuntimeException());
        }
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
