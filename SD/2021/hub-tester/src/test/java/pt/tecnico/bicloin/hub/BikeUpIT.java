package pt.tecnico.bicloin.hub;


import io.grpc.StatusRuntimeException;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.TopUpRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.BikeUpRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.BikeUpResponse;

import static io.grpc.Status.INVALID_ARGUMENT;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class BikeUpIT extends HubIT {

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

        frontend.topUp(topUpRequest);
    }

    @Test
    public void bikeUpOKTest() {

        init();

        BikeUpRequest request = BikeUpRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        BikeUpResponse response = frontend.bikeUp(request);

        assertEquals(BikeUpResponse.Response.OK, response.getResponse());
    }

    @Test
    public void invalidUserBikeUpTest() {

        BikeUpRequest request = BikeUpRequest.newBuilder()
                .setUsername("test")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.bikeUp(request))
                        .getStatus()
                        .getCode());
    }

    @Test
    public void invalidStationBikeUpTest() {

        BikeUpRequest request = BikeUpRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("test")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.bikeUp(request))
                        .getStatus()
                        .getCode());
    }

    @Test
    public void outOfBoundsBikeUpTest() {

        init();

        BikeUpRequest request = BikeUpRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("istt")
                .build();

        BikeUpResponse response = frontend.bikeUp(request);

        assertEquals(BikeUpResponse.Response.OUT_OF_BOUNDS, response.getResponse());
    }

    @Test
    public void noBicloinsBikeUpTest() {

        BikeUpRequest request = BikeUpRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        BikeUpResponse response = frontend.bikeUp(request);

        assertEquals(BikeUpResponse.Response.INSUFFICIENT_BICLOINS, response.getResponse());
    }

    @Test
    public void twoBikesBikeUpTest() {

        init();

        BikeUpRequest request = BikeUpRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7369)
                .setLong(-9.1366)
                .setStation("ista")
                .build();

        frontend.bikeUp(request);

        BikeUpResponse response = frontend.bikeUp(request);

        assertEquals(BikeUpResponse.Response.HAS_BIKE, response.getResponse());
    }

    @Test
    public void noBikesBikeUpTest() {

        init();

        BikeUpRequest request = BikeUpRequest.newBuilder()
                .setUsername("alice")
                .setLat(38.7097)
                .setLong(-9.1336)
                .setStation("cate")
                .build();

        BikeUpResponse response = frontend.bikeUp(request);

        assertEquals(BikeUpResponse.Response.NO_BIKES, response.getResponse());
    }
}