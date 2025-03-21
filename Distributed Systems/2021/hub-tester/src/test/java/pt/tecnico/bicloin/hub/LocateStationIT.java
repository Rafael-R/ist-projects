package pt.tecnico.bicloin.hub;


import org.junit.jupiter.api.Test;
import pt.tecnico.bicloin.hub.grpc.Hub.StationData;
import pt.tecnico.bicloin.hub.grpc.Hub.LocateStationRequest;
import pt.tecnico.bicloin.hub.grpc.Hub.LocateStationResponse;

import static org.junit.jupiter.api.Assertions.assertEquals;


public class LocateStationIT extends HubIT {

    @Test
    public void locateStationOKTest() {
        LocateStationRequest request = LocateStationRequest.newBuilder()
                .setLat(38.7380)
                .setLong(-9.3000)
                .setNumStations(2)
                .build();

        LocateStationResponse response = frontend.locateStation(request);

        StationData istt = StationData.newBuilder()
                .setName("istt")
                .setLat(38.7372)
                .setLong(-9.3023)
                .setDocks(20)
                .setPrize(4)
                .setBikes(12)
                .setDistance(218)
                .build();

        StationData stao = StationData.newBuilder()
                .setName("stao")
                .setLat(38.6867)
                .setLong(-9.3124)
                .setDocks(30)
                .setPrize(3)
                .setBikes(20)
                .setDistance(5805)
                .build();

        assertEquals(istt, response.getStation(0));
        assertEquals(stao, response.getStation(1));
    }
}
