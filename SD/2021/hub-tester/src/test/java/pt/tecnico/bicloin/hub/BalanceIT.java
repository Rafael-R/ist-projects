package pt.tecnico.bicloin.hub;


import io.grpc.StatusRuntimeException;
import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.BalanceRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.BalanceResponse;

import static io.grpc.Status.INVALID_ARGUMENT;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class BalanceIT extends HubIT {

    @Test
    public void balanceOKTest() {
        BalanceRequest request = BalanceRequest.newBuilder()
                .setUsername("alice")
                .build();

        BalanceResponse response = frontend.balance(request);
        assertEquals(0, response.getBalance());
    }

    @Test
    public void invalidUserBalanceTest() {
        BalanceRequest request = BalanceRequest.newBuilder()
                .setUsername("test")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.balance(request))
                        .getStatus()
                        .getCode());
    }
}
