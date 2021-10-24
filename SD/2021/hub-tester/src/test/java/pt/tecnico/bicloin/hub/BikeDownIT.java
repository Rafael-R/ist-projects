package pt.tecnico.bicloin.hub;


import io.grpc.StatusRuntimeException;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.TopUpRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.BikeUpRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.BikeDownRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.BikeDownResponse;

import static io.grpc.Status.INVALID_ARGUMENT;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class BikeDownIT extends HubIT {

    @BeforeEach
    public void setUp() {
        frontend.initRec(request);
    }

    public void init()  {
        TopUpRequest topUpRequest = TopUpRequest.newBuilder()
                .setUsername("alice")
                .setAmount(15)
                .setPhoneNumber("+35191102030")
                .build();

        BikeUpRequest bikeUpRequest = BikeUpRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        frontend.topUp(topUpRequest);
        frontend.bikeUp(bikeUpRequest);

    }

    @Test
    public void bikeDownOKTest() {

        init();

        BikeDownRequest request = BikeDownRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        BikeDownResponse response = frontend.bikeDown(request);

        assertEquals(BikeDownResponse.Response.OK, response.getResponse());
    }

    @Test
    public void invalidUserBikeDownTest() {

        BikeDownRequest request = BikeDownRequest.newBuilder()
                .setUsername("test")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.bikeDown(request))
                        .getStatus()
                        .getCode());
    }

    @Test
    public void invalidStationBikeDownTest() {

        BikeDownRequest request = BikeDownRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("test")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.bikeDown(request))
                        .getStatus()
                        .getCode());
    }

    @Test
    public void outOfBoundsBikeDownTest() {

        init();

        BikeDownRequest request = BikeDownRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("istt")
                .build();

        BikeDownResponse response = frontend.bikeDown(request);

        assertEquals(BikeDownResponse.Response.OUT_OF_BOUNDS, response.getResponse());
    }

    @Test
    public void noBikeBikeDownTest() {

        BikeDownRequest request = BikeDownRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        BikeDownResponse response = frontend.bikeDown(request);

        assertEquals(BikeDownResponse.Response.NO_BIKE, response.getResponse());
    }

    @Test
    public void noDocksBikeDownTest() {

        init();

        BikeDownRequest request = BikeDownRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7376)
                .setLong(-9.1545)
                .setStation("gulb")
                .build();

        BikeDownResponse response = frontend.bikeDown(request);

        assertEquals(BikeDownResponse.Response.NO_DOCKS, response.getResponse());
    }
}