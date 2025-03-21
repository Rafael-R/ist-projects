package pt.tecnico.bicloin.hub;


import io.grpc.StatusRuntimeException;
import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.PingRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.PingResponse;

import static io.grpc.Status.INVALID_ARGUMENT;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class PingIT extends HubIT{

    @Test
    public void pingOKTest() {
        PingRequest request = PingRequest.newBuilder()
                .setInputText("friend")
                .build();

        PingResponse response = frontend.ping(request);
        assertEquals("Hello friend!", response.getOutputText());
    }

    @Test
    public void emptyPingTest() {
        PingRequest request = PingRequest.newBuilder()
                .setInputText("")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.ping(request))
                        .getStatus()
                        .getCode());
    }
}
