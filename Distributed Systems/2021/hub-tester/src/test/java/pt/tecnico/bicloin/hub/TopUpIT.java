package pt.tecnico.bicloin.hub;


import io.grpc.StatusRuntimeException;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.TopUpRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.TopUpResponse;

import static io.grpc.Status.INVALID_ARGUMENT;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class TopUpIT extends HubIT {

    @BeforeEach
    public void setUp() {
        frontend.initRec(request);
    }

    @Test
    public void topUpOKTest() {
        setUp();

        TopUpRequest request = TopUpRequest.newBuilder()
                .setUsername("alice")
                .setAmount(15)
                .setPhoneNumber("+35191102030")
                .build();

        TopUpResponse response = frontend.topUp(request);
        assertEquals(150, response.getBalance());
    }

    @Test
    public void invalidUserTopUpTest() {
        TopUpRequest request = TopUpRequest.newBuilder()
                .setUsername("test")
                .setAmount(15)
                .setPhoneNumber("+35191102030")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.topUp(request))
                        .getStatus()
                        .getCode());
    }

    @Test
    public void invalidAmountTopUpTest() {
        TopUpRequest request = TopUpRequest.newBuilder()
                .setUsername("alice")
                .setAmount(21)
                .setPhoneNumber("+35191102030")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.topUp(request))
                        .getStatus()
                        .getCode());
    }

    @Test
    public void invalidPhoneTopUpTest() {
        TopUpRequest request = TopUpRequest.newBuilder()
                .setUsername("alice")
                .setAmount(15)
                .setPhoneNumber("+35191111111")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.topUp(request))
                        .getStatus()
                        .getCode());
    }
}
