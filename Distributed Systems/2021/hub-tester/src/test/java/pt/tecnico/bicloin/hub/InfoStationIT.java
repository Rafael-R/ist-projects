package pt.tecnico.bicloin.hub;


import io.grpc.StatusRuntimeException;
import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.StationData;
import pt.tecnico.bicloin.hub.grpc.Hub.InfoStationRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.InfoStationResponse;

import static io.grpc.Status.INVALID_ARGUMENT;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class InfoStationIT extends HubIT {

    @Test
    public void infoStationOKTest() {
        InfoStationRequest request = InfoStationRequest.newBuilder()
                .setStation("istt")
                .build();

        InfoStationResponse response = frontend.infoStation(request);

        StationData istt = StationData.newBuilder()
                .setName("IST Taguspark")
                .setLat(38.7372)
                .setLong(-9.3023)
                .setDocks(20)
                .setPrize(4)
                .setBikes(12)
                .setLifts(0)
                .setReturns(0)
                .build();

        assertEquals(istt, response.getStation());
    }

    @Test
    public void invalidStationInfoStationTest() {
        InfoStationRequest request = InfoStationRequest.newBuilder()
                .setStation("test")
                .build();

        assertEquals(
                INVALID_ARGUMENT.getCode(),
                assertThrows(
                        StatusRuntimeException.class, () -> frontend.infoStation(request))
                        .getStatus()
                        .getCode());
    }
}
